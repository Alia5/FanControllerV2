
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

#include "Device.h"


Device::Device()
{
	msgbox.setWindowTitle("FanController");
	msgbox2.setWindowTitle("FanController");
}


Device::~Device()
{
	close();
}


bool Device::init()
{
	devicehandle = hid_open(0x16c0, 0x27d9, NULL);
	noDevice = false;
	if (devicehandle == NULL)
	{
		noDevice = true;
		msgbox.setText("Error Device not found!");
		msgbox.show();
	}

	return noDevice;
}



bool Device::connected()
{
	return !noDevice;
}

void Device::close()
{
	hid_close(devicehandle);
}

bool Device::update(UpdateValues& FanValues)
{
	if (noDevice)
	{
		devicehandle = hid_open(0x16c0, 0x27d9, NULL);
	}

	if (devicehandle != NULL)
	{
		memset(buf, NULL, 33);	  //clear buffer
		buf[0] = 0x00;			  //hid stuff
		buf[1] = 0x81;			  //reportbyte to device

		for (int i = 0, j = 2; i < 6; i++, j++)
		{
			if (FanValues.FanValues[i] == 0)
			{
				buf[j] = 0x01;					//0x01 is off! not zero!
			} else {
				buf[j] = FanValues.FanValues[i];
			}
		}

		res = hid_send_feature_report(devicehandle, buf, 33);		//send the data
		if (res == -1)
		{
			disconnect_on_error();
			return false;
		}

		hid_read(devicehandle, buf, 8);				//read rpm values from device //we need to read 8 bytes, instead of 7 (reportbyte + 6 rpm values) because of hidapi

		if (buf[0] != 0x01 && noDevice == false)	//device not found, 0x01 is reportbyte from device
		{
			disconnect_on_error();
			return false;
			for (int i = 0; i < 6; i++)
			{
				FanValues.RPMValues[i] = 0;
			}
			return false;
		}

		for (int i = 0, j = 1; i < 6; i++, j++)
		{
			FanValues.RPMValues[i] = buf[j] * 30;	//you guessed it. max rpm = 255*30 = 7650;
		}

	} else {
		noDevice = true;
		return false;
	}

	msgbox2.close();				//if we autoreconnected, close the messagebox... if not already
	return true;
}

void Device::disconnect_on_error()
{
	noDevice = true;
	hid_close(devicehandle);
	devicehandle = NULL;
	msgbox2.setText("Error while communicating with device");
	msgbox2.show();
}

Device::deviceinfo Device::getDeviceInfo()
{
	deviceinfo result;
	if (devicehandle != NULL)
	{
		wchar_t string[255];
		hid_get_manufacturer_string(devicehandle, string, 255);

		result.Manufacturer = QString::fromWCharArray(string);

		hid_get_product_string(devicehandle, string, 255);

		result.Product = QString::fromWCharArray(string);

		hid_get_serial_number_string(devicehandle, string, 255);

		result.Serial = QString::fromWCharArray(string);
	}
	else {
		result.Manufacturer = "Error";
		result.Product = "Error";
		result.Serial = "Error";
	}
	return result;
}