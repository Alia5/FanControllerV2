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
		fspStatusPage.fsFanStatus[i].Fanslider	= UI.statustab->findChild<QSlider *>("vS_Fan_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].Percentage = UI.statustab->findChild<QLabel *>("l_FanPerc_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].RPM		= UI.statustab->findChild<QLabel *>("l_FanRPM_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].Voltage	= UI.statustab->findChild<QLabel *>("l_FanVoltage_" + QString::fromStdString(std::to_string(i + 1)));
		fspStatusPage.fsFanStatus[i].KeepOff	= UI.statustab->findChild<QCheckBox *>("cB_keepOff_" + QString::fromStdString(std::to_string(i + 1)));
	}
}

void ui_fan::initAutoPage(Ui::FanControllerV2Class& UI, AutoPages& fapAutoPage)
{
	//QMap<double, double> D;

	//D.insert(0, 0);
	//D.insert(60, 50);
	//D.insert(75, 100);
	//D.insert(100, 100);

	QCPScatterStyle myScatter;
	myScatter.setShape(QCPScatterStyle::ssCircle);
	myScatter.setPen(QPen(QBrush(Qt::red), 2));
	myScatter.setBrush(Qt::white);
	myScatter.setSize(15);

	for (int i = 0; i < 6; i++)
	{

		//fapAutoPage.apAutoPage[i].Data = D;

		fapAutoPage.apAutoPage[i].ComboBox = UI.autosettings->findChild<QComboBox *>("coB_Fan_" + QString::fromStdString(std::to_string(i + 1)));
		fapAutoPage.apAutoPage[i].CustomPlot = UI.autosettings->findChild<QCustomPlot *>("cPl_Fan_" + QString::fromStdString(std::to_string(i + 1)));
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setRange(0, 100);
		fapAutoPage.apAutoPage[i].CustomPlot->xAxis->setLabel(QString::fromStdWString(L"Temperature (°C)"));
		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setRange(0, 100);
		fapAutoPage.apAutoPage[i].CustomPlot->yAxis->setLabel(QString::fromStdWString(L"Fanspeed (%)"));
		fapAutoPage.apAutoPage[i].CustomPlot->setInteraction(QCP::iSelectPlottables, true);
		fapAutoPage.apAutoPage[i].CustomPlot->addGraph();
		fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setPen(QPen(QBrush(QColor(255, 0, 0)), 2));
		fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setSelectedPen(QPen(QBrush(QColor(255, 0, 0)), 2));
		fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setScatterStyle(myScatter);
		//fapAutoPage.apAutoPage[i].CustomPlot->graph(0)->setData(fapAutoPage.apAutoPage[i].Data.keys().toVector(), fapAutoPage.apAutoPage[i].Data.values().toVector());
		fapAutoPage.apAutoPage[i].CustomPlot->replot();
	}
}
