#pragma once

#include "ui_fancontrollerv2.h"

class ui_fan
{
public:
	ui_fan();
	~ui_fan();

	static struct FanStatus
	{
		QSlider* Fanslider;
		QLabel* Percentage;
		QLabel* RPM;
		QLabel* Voltage;
		QCheckBox* KeepOff;
	};

	static struct FanStatusPage
	{
		FanStatus fsFanStatus[6];
	};

	static void initStatus(Ui::FanControllerV2Class& UI, FanStatusPage& fspStatusPage);

	static struct AutoPage
	{
		int ComboBoxIndex;
		QComboBox *ComboBox;
		QCustomPlot *CustomPlot;
		QMap<double, double> Data;
		double selectedKey;
	};

	static struct AutoPages
	{
		AutoPage apAutoPage[6];
	};

	static void initAutoPage(Ui::FanControllerV2Class& UI, AutoPages& fapStatusPage);

};

