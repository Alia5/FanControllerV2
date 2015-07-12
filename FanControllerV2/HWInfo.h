
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

#include <QSharedMemory>
#include <QListWidget>
#include <QMessageBox>
#include "SettingsHandler.h"
#include "ui_fan.h"


class HWInfo
{
private:
	QSharedMemory sharedMemory;
	QString InfoString;

	int sensorValues;
	QString *tempValues = NULL;
	
	QListWidget *lW_Status;

	QString TemperatureEntry;

	bool readSucces = false;
	
	SettingsHandler* Settings;

	ui_fan::AutoPages* AutoPages;

	QMessageBox box;

	bool errorShown = false;

	char temp[4096];

	int waiter = 0;

	//hwinfo64 sensor api shouldn't be made publicly.. so i only provided a proprietary dll...
	//since this program is gpl licensed, i use the plugin as an optinal one and load it dynamically, so it could live without the application
	typedef void(__cdecl* _readHWiNFOTemps)(char* c_str, int len);
	_readHWiNFOTemps readHWiNFOTemps = NULL;

public:
	HWInfo();
	~HWInfo();

	bool init(SettingsHandler& settings, QListWidget* lw_status, ui_fan::AutoPages& autoPages);
	bool update();
};
