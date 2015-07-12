#pragma once


#include <QMessageBox>
#include "hid/hidapi.h"

class Device
{
public:
	Device();
	~Device();
	static struct UpdateValues
	{
		int FanValues[6];
		int RPMValues[6];
	};

	static struct deviceinfo
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

