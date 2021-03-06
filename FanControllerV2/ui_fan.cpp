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

#include "ui_fan.h"

ui_fan::ui_fan()
{
}

ui_fan::~ui_fan()
{
}

void ui_fan::initStatus(Ui::FanControllerV2Class& UI, FanStatusPage& fspStatusPage)
{
	for (int i = 0; i < 6; i++)
	{
		fspStatusPage.fsFanStatus[i].Fanslider = UI.statustab->findChild<QSlider *>("vS_Fan_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].Percentage = UI.statustab->findChild<QLabel *>("l_FanPerc_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].RPM = UI.statustab->findChild<QLabel *>("l_FanRPM_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].Voltage = UI.statustab->findChild<QLabel *>("l_FanVoltage_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].KeepOff = UI.statustab->findChild<QCheckBox *>("cB_keepOff_" + QString::fromStdString(std::to_string(i + 1)));
	}
}

void ui_fan::initAutoPage(Ui::FanControllerV2Class& UI, AutoPages& fapAutoPage)
{
	QCPScatterStyle myScatter;
	myScatter.setShape(QCPScatterStyle::ssCircle);
	myScatter.setPen(QPen(QBrush(Qt::red), 2));
	myScatter.setBrush(Qt::red);
	myScatter.setSize(10);

	for (int i = 0; i < 6; i++)
	{
		fapAutoPage.apAutoPage[i].ComboBox = UI.autosettings->findChild<QComboBox *>("coB_Fan_" + QString::fromStdString(std::to_string(i + 1)));
		fapAutoPage.apAutoPage[i].CustomPlot = UI.autosettings->findChild<QCustomPlot *>("cPl_Fan_" + QString::fromStdString(std::to_string(i + 1)));
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setRange(0, 100);
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setAutoTickStep(false);
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setTickStep(10);
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setAutoSubTicks(false);
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setSubTickCount(3);

		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setRange(0, 100);
		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setAutoTickStep(false);
		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setTickStep(10);
		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setAutoSubTicks(false);
		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setSubTickCount(3);
		fapAutoPage.apAutoPage[i].CustomPlot->setInteraction(QCP::iSelectPlottables, true);
		fapAutoPage.apAutoPage[i].CustomPlot->addGraph();
		fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setPen(QPen(QBrush(QColor(0, 128, 255)), 1));
		fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setSelectedPen(QPen(QBrush(QColor(0, 128, 255)), 1));
		fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setBrush(QBrush(QColor(0, 128, 255, 20)));

		fapAutoPage.apAutoPage[i].CustomPlot->addGraph();
		fapAutoPage.apAutoPage[i].CustomPlot->graph(1)->setPen(QPen(QBrush(QColor(255, 0, 0)), 2));
		fapAutoPage.apAutoPage[i].CustomPlot->graph(1)->setSelectedPen(QPen(QBrush(QColor(255, 0, 0)), 2));
		fapAutoPage.apAutoPage[i].CustomPlot->graph(1)->setScatterStyle(myScatter);

		fapAutoPage.apAutoPage[i].CustomPlot->replot();
	}
}