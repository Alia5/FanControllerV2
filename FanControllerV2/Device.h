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

#include <QMessageBox>
#include "hid/hidapi.h"

class Device
{
public:
	Device();
	~Device();
	struct UpdateValues
	{
		int FanValues[6];
		int RPMValues[6];
	};

	struct deviceinfo
	{
		QString Manufacturer;
		QString Product;
		QString Serial;
	};

	bool init();
	void close();

	bool connected();

	//handy little function.. autoreconnects if needed ;)
	bool update(UpdateValues& updatevals);

	deviceinfo getDeviceInfo();

private:
	//HID STUFF
	int res;
	unsigned char buf[33];		//32byte long buffer... 33 byte since the device descripter requires it
	wchar_t wstr[255];
	hid_device *devicehandle;
	///////////

	void disconnect_on_error();

	QMessageBox msgbox;
	QMessageBox msgbox2;
	bool noDevice = false;
};
