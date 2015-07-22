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

#include "SettingsHandler.h"

SettingsHandler::SettingsHandler()
{
	settingspath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#ifdef _WIN32
	settingspath.replace("Local", "Roaming");
#endif
	QDir dir(settingspath);
	if (!dir.exists()) {
		dir.mkpath(".");
		CreateSettings();
	}
	ReadSettings();
}

SettingsHandler::~SettingsHandler()
{
	WriteSettings();
}

void SettingsHandler::ReadSettings()
{
	QFile file(settingspath + "/general.dat");
	QDataStream filestream(&file);

	if (!file.exists())
		CreateSettings();
	file.open(QIODevice::ReadWrite);
	filestream >> Data.versionnumber >> Data.autoRun >> Data.startMini >> Data.useAIDA >> Data.maxCPUTemp >> Data.indexOfCPUTemp
		>> Data.useSliders >> Data.useDials >> Data.winSizeX >> Data.winSizeY >> Data.waitfor >> Data.fan_hysterisis;		//autorun, startmini, useAIDA, maxCPUTemp
	file.close();

	/////////

	//consider prompting user for clearing settings here, IF settings end up being not backwards compatible

	//let this sit here for at least until version 207
	if (Data.versionnumber == 203)
	{
		CreateSettings();
		QMessageBox::warning(0, "FanControll", "Please check your Settings since they are set uncorrectly due to the update!\nSorry for the inconvinience");
	}

	/////////

	Data.versionnumber = VersionNumber;

	file.setFileName(settingspath + "/fansettings.dat");
	if (!file.exists())
		CreateSettings();
	file.open(QIODevice::ReadWrite);
	for (int i = 0; i < 6; i++)
	{
		filestream >> Data.CalibrationData[i].calibrated >> Data.CalibrationData[i].value;
	}
	file.close();

	file.setFileName(settingspath + "/automode.dat");
	if (!file.exists())
		CreateSettings();
	file.open(QIODevice::ReadWrite);
	for (int i = 0; i < 6; i++)
	{
		filestream >> Data.AutoPages.apAutoPage[i].Item >> Data.AutoPages.apAutoPage[i].Data;
	}
	file.close();
}

void SettingsHandler::WriteSettings()
{
	QFile file(settingspath + "/fansettings.dat");
	QDataStream filestream(&file);
	file.open(QIODevice::ReadWrite);
	for (int i = 0; i < 6; i++)
	{
		filestream << Data.CalibrationData[i].calibrated << Data.CalibrationData[i].value;
	}
	file.close();

	file.setFileName(settingspath + "/general.dat");
	file.open(QIODevice::ReadWrite);
	filestream << VersionNumber << Data.autoRun << Data.startMini << Data.useAIDA << Data.maxCPUTemp << Data.indexOfCPUTemp
		<< Data.useSliders << Data.useDials << Data.winSizeX << Data.winSizeY << Data.waitfor << Data.fan_hysterisis;		//autorun, startmini, useAIDA, maxCPUTemp, index, winsize
	file.close();

	file.setFileName(settingspath + "/automode.dat");
	file.open(QIODevice::ReadWrite);
	QString ItemText;
	for (int i = 0; i < 6; i++)
	{
		ItemText = Data.AutoPages.apAutoPage[i].ComboBox->currentText();
		filestream << ItemText << Data.AutoPages.apAutoPage[i].Data;
	}
	file.close();
}

void SettingsHandler::CreateSettings()
{
	QDir dir(settingspath);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	QFile file(settingspath + "/fansettings.dat");
	QDataStream filestream(&file);

	file.open(QIODevice::ReadWrite);

	for (int i = 0; i < 6; i++)
	{
		filestream << (bool)false << (int)0;
	}

	file.close();

	file.setFileName(settingspath + "/automode.dat");
	file.open(QIODevice::ReadWrite);
	QMap<double, double> automode;
	automode.insert(0, 0);
	automode.insert(50, 50);
	automode.insert(60, 100);
	automode.insert(100, 100);

	for (int i = 0; i < 6; i++)
	{
		filestream << QString("UNINIT") << automode;
	}
	file.close();

	file.close();

	file.setFileName(settingspath + "/general.dat");
	file.open(QIODevice::ReadWrite);
	filestream << VersionNumber << (bool)true << (bool)false << (bool)true << 72 << 0 << bool(true) << bool(false) << 680 << 400 << (bool)false << 5;		//versionnumber, autorun, startmini, useAIDA, maxCPUTemp, index, winsize
	file.close();
}