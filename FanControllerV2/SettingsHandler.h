#pragma once

#include <QString>
#include <QStandardPaths>
#include <Qdir>

#include "ui_fan.h"

#define VersionNumber 200

class SettingsHandler
{
private:
	static struct FanCalibration
	{
		bool calibrated;
		int value;
	};


	QString settingspath;
	void CreateSettings();

public:

	static struct SettingsData
	{
		int versionnumber;
		bool autoRun;
		bool startMini;
		bool useAIDA;
		int maxCPUTemp;
		int indexOfCPUTemp;
		bool useSliders;
		bool useDials;
		int winSizeX;
		int winSizeY;
		FanCalibration CalibrationData[6];
		ui_fan::AutoPages AutoPages;
	};

	SettingsHandler();
	~SettingsHandler();

	SettingsData Data;

	void ReadSettings();
	void WriteSettings();

};

