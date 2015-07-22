
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

#include "ui_fancontrollerv2.h"

class ui_fan
{
public:
	ui_fan();
	~ui_fan();

	struct FanStatus
	{
		QSlider* Fanslider;
		QLabel* Percentage;
		QLabel* RPM;
		QLabel* Voltage;
		QCheckBox* KeepOff;
	};

	struct FanStatusPage
	{
		FanStatus fsFanStatus[6];
	};

	static void initStatus(Ui::FanControllerV2Class& UI, FanStatusPage& fspStatusPage);

	struct AutoPage
	{
		QString Item;
		QComboBox *ComboBox;
		QCustomPlot *CustomPlot;
		QMap<double, double> Data;
		double selectedKey;
	};

	struct AutoPages
	{
		AutoPage apAutoPage[6];
	};

	static void initAutoPage(Ui::FanControllerV2Class& UI, AutoPages& fapStatusPage);

};

