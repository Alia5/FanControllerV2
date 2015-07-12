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
	typedef void(__stdcall* _readHWiNFOTemps)(char*, int);
	_readHWiNFOTemps readHWiNFOTemps;

public:
	HWInfo();
	~HWInfo();

	bool init(SettingsHandler& settings, QListWidget* lw_status, ui_fan::AutoPages& autoPages);
	bool update();
};
