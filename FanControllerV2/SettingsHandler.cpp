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
	filestream >> Data.versionnumber >> Data.autoRun >> Data.startMini >> Data.useAIDA >> Data.maxCPUTemp >> Data.indexOfCPUTemp >> Data.useSliders >> Data.useDials >> Data.winSizeX >> Data.winSizeY;		//autorun, startmini, useAIDA, maxCPUTemp
	file.close();	

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
		filestream >> Data.AutoPages.apAutoPage[i].ComboBoxIndex >> Data.AutoPages.apAutoPage[i].Data;
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
	filestream << VersionNumber << Data.autoRun << Data.startMini << Data.useAIDA << Data.maxCPUTemp << Data.indexOfCPUTemp << Data.useSliders << Data.useDials << Data.winSizeX << Data.winSizeY;		//autorun, startmini, useAIDA, maxCPUTemp, index, winsize
	file.close();

	file.setFileName(settingspath + "/automode.dat");
	file.open(QIODevice::ReadWrite);
	for (int i = 0; i < 6; i++)
	{		
		filestream << Data.AutoPages.apAutoPage[i].ComboBox->currentIndex() << Data.AutoPages.apAutoPage[i].Data;
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
		filestream << int(-1) << automode;
	}
	file.close();

	file.close();

	file.setFileName(settingspath + "/general.dat");
	file.open(QIODevice::ReadWrite);
	filestream << VersionNumber << (bool)true << (bool)false << (bool)true << 72 << 0 << bool(true) << bool(false) << 680 << 400;		//versionnumber, autorun, startmini, useAIDA, maxCPUTemp, index, winsize
	file.close();

}