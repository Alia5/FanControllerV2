
/*
* Project: FanControlV2 host apllication for 6_channel_usb_fancontroller
* Author: Peter Repukat
* Copyright: (c) 2015 Peter Repukat
* License: GNU GPL v3 (see License.txt)

* QCustomPlot:
* Author: Emanuel Eichhammer
* Copyright (C) 2011-2015 Emanuel Eichhammer
* License: GNU GPL v3 (see License.txt)

* HIDAPI:
* Author:  Alan Ott  Signal 11 Software
* Copyright (C) 2009 Alan Ott  Signal 11 Software
* License: GNU GPL v3 (see License.txt)

* Qt Toolkit:
* Copyright (C) The Qt Company
* License: GNU LGPL v3 (see qt-license.txt)
*/

#include "HWInfo.h"


HWInfo::HWInfo()
{
	box.setWindowTitle("FanController");
#ifdef _WIN32
	readHWiNFOTemps = (_readHWiNFOTemps)GetProcAddress((HMODULE)LoadLibraryA("HWiNFO64.dll"), "readHWiNFOTemps");
	if (readHWiNFOTemps == NULL)
	{
		QMessageBox::information(0, "FanControl", "HWiNFO Plugin not found!\nReading Sensorvalues from HWiNFO may not be possible!");
	}
#endif
}


HWInfo::~HWInfo()
{
	delete[] tempValues;
}


bool HWInfo::init(SettingsHandler& settings, QListWidget* lw_status, ui_fan::AutoPages& autoPages)
{
#ifdef __linux__
	return false;
#endif

	lW_Status = lw_status;
	Settings = &settings;
	AutoPages = &autoPages;

	for (int j = 0; j < 6; j++)
	{
		autoPages.apAutoPage[j].ComboBox->clear();
	}

	if (Settings->Data.useAIDA)
	{
		sharedMemory.setNativeKey("AIDA64_SensorValues");
		if (sharedMemory.attach(QSharedMemory::AccessMode::ReadOnly))
		{
			InfoString = QString((char*)sharedMemory.data());
			sharedMemory.detach();
			sensorValues = InfoString.count("<temp>");
			if (tempValues == NULL)
				tempValues = new QString[sensorValues];
			for (int i = 0; i < sensorValues; i++)
			{
				tempValues[i] = InfoString.section("<temp>", i+1, i+1);
				tempValues[i].remove("/");

				TemperatureEntry = tempValues[i].section("<label>", 1, 1) + ": " + tempValues[i].section("<value>", 1, 1) + QString::fromStdWString(L"°C");
				lW_Status->addItem(TemperatureEntry);
				lW_Status->item(i)->setToolTip(TemperatureEntry);

				for (int j = 0; j < 6; j++)
				{
					QString boxentry = TemperatureEntry;
					boxentry.chop(6);
					if (boxentry.at(boxentry.length()-1) == ':')
						boxentry.chop(1);
					autoPages.apAutoPage[j].ComboBox->addItem(boxentry);
				}

			}
			for (int i = 0; i < sensorValues; i++)
			{
				if (lW_Status->item(i)->text().contains("CPU: "))
				{
					Settings->Data.indexOfCPUTemp = i;
					break;
				}
			}
			for (int j = 0; j < 6; j++)
			{
				if (Settings->Data.AutoPages.apAutoPage[j].ComboBoxIndex >= 0)
					autoPages.apAutoPage[j].ComboBox->setCurrentIndex(Settings->Data.AutoPages.apAutoPage[j].ComboBoxIndex);
				else
					autoPages.apAutoPage[j].ComboBox->setCurrentIndex(Settings->Data.indexOfCPUTemp);
			}

			readSucces = true;
			waiter = 10;
		} else {
			readSucces = false;
		}
	} else if (readHWiNFOTemps != NULL){
		readHWiNFOTemps(temp, 4096);
		InfoString = QString::fromLatin1(temp);

		int indexOffset = 0;

		if (InfoString == "DEAD")					//memory is dead...
		{
			readSucces = false;
			return readSucces;
		}
		if (InfoString == "CLOSED")					//either HWiNFO64 or the sensor panel is closed
		{

			readSucces = false;
			return readSucces;
		}


		sensorValues = InfoString.count("<sep>");
		if (tempValues == NULL)
			tempValues = new QString[sensorValues];
		for (int i = 0; i < sensorValues; i++)
		{
			tempValues[i] = InfoString.section("<sep>", i, i);

			TemperatureEntry = tempValues[i].remove("<val>");

			if (TemperatureEntry.contains(QString::fromStdWString(L"°C")))	//clean temperaturereadings up a bit...
				TemperatureEntry.remove(QRegExp("\\[(.*)\\]"));

			lW_Status->addItem(TemperatureEntry);

			lW_Status->item(i)->setToolTip(TemperatureEntry);

			if (TemperatureEntry.contains(QString::fromStdWString(L"°C")))
			{
				for (int j = 0; j < 6; j++)
				{
					QString boxentry = TemperatureEntry;
					boxentry.chop(5);
					boxentry.remove("  ");
					while (boxentry.at(boxentry.length() - 1) == ':' || boxentry.at(boxentry.length() - 1) == ' ')
						boxentry.chop(1);
					autoPages.apAutoPage[j].ComboBox->addItem(boxentry);
				}
			}
		}
		for (int i = 0; i < sensorValues; i++)
		{
			if (!lW_Status->item(i)->text().contains(QString::fromStdWString(L"°C")))
				indexOffset++;
			if (lW_Status->item(i)->text().contains("CPU: ") || lW_Status->item(i)->text().contains("CPU ("))
			{
				Settings->Data.indexOfCPUTemp = i;
				break;
			}
		}
		for (int j = 0; j < 6; j++)
		{
			if (Settings->Data.AutoPages.apAutoPage[j].ComboBoxIndex >= 0)
				autoPages.apAutoPage[j].ComboBox->setCurrentIndex(Settings->Data.AutoPages.apAutoPage[j].ComboBoxIndex);
			else
				autoPages.apAutoPage[j].ComboBox->setCurrentIndex(Settings->Data.indexOfCPUTemp - indexOffset);
		}

		readSucces = true;
		waiter = 10;
	}
	if (readSucces)
		errorShown = false;
	return readSucces;
}

bool HWInfo::update()
{
#ifdef __linux__
	return false;
#endif

	if (Settings->Data.useAIDA)
	{
		sharedMemory.setNativeKey("AIDA64_SensorValues");
		if (sharedMemory.attach(QSharedMemory::AccessMode::ReadOnly))
		{
			InfoString = QString((char*)sharedMemory.data());
			sharedMemory.detach();

			if (sensorValues != InfoString.count("<temp>"))     //if we have new hardware at runtime (eg, e-sata stuff, we need to reinit..) //also if we switch sensor source or something else goes wrong
			{
				delete[] tempValues;
				tempValues = NULL;
				sensorValues = 0;
				lW_Status->clear();
				return init(*Settings,lW_Status, *AutoPages);
			}

			for (int i = 0; i < sensorValues; i++)
			{
				tempValues[i] = InfoString.section("<temp>", i + 1, i + 1);
				tempValues[i].remove("/");

				TemperatureEntry = tempValues[i].section("<label>", 1, 1) + ": " + tempValues[i].section("<value>", 1, 1) + QString::fromStdWString(L"°C");
				lW_Status->item(i)->setText(TemperatureEntry);
				lW_Status->item(i)->setToolTip(TemperatureEntry);
			}
			readSucces = true;
		} else {
			if (!errorShown)
			{
				if (waiter < 10)
				{
					waiter++;
				}
				else {
					if (!errorShown)
					{
						box.setText("Error while reading data from AIDA64!");
						box.show();
					}
					errorShown = true;
					delete[] tempValues;
					tempValues = NULL;
					sensorValues = 0;
					lW_Status->clear();
				}
			}
			readSucces = false;
		}
	} else if (readHWiNFOTemps != NULL) {
		memset(temp, NULL, 4096);
		readHWiNFOTemps(temp, 4096);
		InfoString = QString::fromLatin1(temp);

		if (InfoString == "DEAD")					//memory is dead...
		{
			if (waiter < 10)
			{
				waiter++;
			}
			else {
				if (!errorShown)
				{
					box.setText("Error while reading data from HWiNFO64!");
					box.show();
				}
				readSucces = false;
				errorShown = true;
				delete[] tempValues;
				tempValues = NULL;
				sensorValues = 0;
				lW_Status->clear();
				return readSucces;
			}
		}
		if (InfoString == "CLOSED")					
		{
			if (waiter < 10)
			{
				waiter++;
			}
			else {
				if (!errorShown)
				{
					box.setText("HWiNFOs Sensor-panel is closed, or HWiNFO is!");
					box.show();
				}
				readSucces = false;
				errorShown = true;
				delete[] tempValues;
				tempValues = NULL;
				sensorValues = 0;
				lW_Status->clear();
				return readSucces;
			}
		}

		if (sensorValues != InfoString.count("<sep>"))		//if we have new hardware at runtime (eg, e-sata stuff, we need to reinit..) //also if we switch sensor source or something else goes wrong
		{
			delete[] tempValues;
			tempValues = NULL;
			sensorValues = 0;
			lW_Status->clear();
			return init(*Settings, lW_Status, *AutoPages);
		}

		for (int i = 0; i < sensorValues; i++)
		{
			tempValues[i] = InfoString.section("<sep>", i, i);

			TemperatureEntry = tempValues[i].remove("<val>");

			if (TemperatureEntry.contains(QString::fromStdWString(L"°C")))	//clean temperaturereadings up a bit...
				TemperatureEntry.remove(QRegExp("\\[(.*)\\]"));

			lW_Status->item(i)->setText(TemperatureEntry);

			lW_Status->item(i)->setToolTip(TemperatureEntry);
			readSucces = true;
		}

	}
	if (readSucces)
		errorShown = false;
	return readSucces;
}
