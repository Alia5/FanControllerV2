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

#pragma once

#include <QString>
#include <QStandardPaths>
#include <Qdir>

#include "ui_fan.h"

#define VersionNumber 204

class SettingsHandler
{
private:
	struct FanCalibration
	{
		bool calibrated;
		int value;
	};

	QString settingspath;
	void CreateSettings();

public:

	struct SettingsData
	{
		int versionnumber;
		bool autoRun;
		bool startMini;
		bool waitfor;
		bool useAIDA;
		int maxCPUTemp;
		int indexOfCPUTemp;
		bool useSliders;
		bool useDials;
		int winSizeX;
		int winSizeY;
		FanCalibration CalibrationData[6];
		ui_fan::AutoPages AutoPages;
		int fan_hysterisis;
	};

	SettingsHandler();
	~SettingsHandler();

	SettingsData Data;

	void ReadSettings();
	void WriteSettings();
};
