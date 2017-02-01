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

#include "fancontrollerv2.h"

FanControllerV2::FanControllerV2(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	msgbox.setWindowTitle("FanController");
	init();
}

void FanControllerV2::init()
{
#ifdef __linux__
	ui.cb_autorun->setEnabled(false);
	ui.rB_Aida->setVisible(false);
	ui.rB_HWiNFO->setVisible(false);
#endif // __linux


#ifdef _WIN32
	QFile file("HWiNFO64.dll");				//since HWiNFO64.dll is optional, if its not present, hide the option from using it
	if (!file.exists())
		ui.rB_HWiNFO->setVisible(false);
#endif

	silent = false;
	freeze = false;

	Instance.setKey("Alia5-FanController");
	Instance.create(1024);
	Instance.attach();
	Instance.lock();
	memset(Instance.data(), NULL, 1024);
	Instance.unlock();

	ui.rB_Manualmode->setChecked(true);

	ticon = QIcon(":/FanControllerV2/icon.png");
	trayIcon.setIcon(ticon);
	trayIconMenu.addAction("Reset to orginal windowsize");
	trayIconMenu.addAction("Quit");

	ui.centralWidget->setWindowIcon(ticon);

	trayIcon.setContextMenu(&trayIconMenu);

	trayIcon.setToolTip("Fan Controller v" + QString::number(float(Settings.Data.versionnumber / 100.f), 'g', 3));
	trayIcon.setVisible(true);

	connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	connect(*trayIconMenu.actions().begin(), SIGNAL(triggered()), this, SLOT(restoreSize()));
	connect(trayIconMenu.actions().at(1), SIGNAL(triggered()), this, SLOT(programClose()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

	this->AutoPages = Settings.Data.AutoPages;
	ui_fan::initStatus(ui, StatusPage);
	ui_fan::initAutoPage(ui, AutoPages);
	for (int i = 0; i < 6; i++)
	{
		connect(StatusPage.fsFanStatus[i].Fanslider, SIGNAL(valueChanged(int)), this, SLOT(SliderValChanged(int)));
		connect(AutoPages.apAutoPage[i].CustomPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(AutoGraphUpdate(QMouseEvent*)));
		connect(AutoPages.apAutoPage[i].CustomPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(AutoGraphUpdate(QMouseEvent*)));
		AutoPages.apAutoPage[i].CustomPlot->graph(1)->setData(AutoPages.apAutoPage[i].Data.keys().toVector(), AutoPages.apAutoPage[i].Data.values().toVector());
		AutoPages.apAutoPage[i].CustomPlot->replot();

		calibrationHelp.shouldCalibrate[i] = false;

		if (Settings.Data.CalibrationData[i].calibrated)
			StatusPage.fsFanStatus[i].Fanslider->setMaximum(255 - Settings.Data.CalibrationData[i].value);

		for (int j = 0; j < 10; j++)
		{
			temps[i][j] = 50;
		}
		initTempGraphs(i);
	}

	device.init();

	ui.cb_autorun->setChecked(Settings.Data.autoRun);
	ui.cb_startmini->setChecked(Settings.Data.startMini);
	ui.cb_waitfor->setChecked(Settings.Data.waitfor);

	if (Settings.Data.startMini && device.connected())
		QTimer::singleShot(0, this, SLOT(hide()));

	ui.rB_Aida->setChecked(Settings.Data.useAIDA);
	if (!ui.rB_Aida->isChecked())
		ui.rB_HWiNFO->setChecked(true);
	ui.sB_MaxTemp->setValue(Settings.Data.maxCPUTemp);

	ui.cb_sliders->toggled(Settings.Data.useSliders);
	ui.cb_dials->toggled(Settings.Data.useDials);
	ui.cb_sliders->setChecked(Settings.Data.useSliders);
	ui.cb_dials->setChecked(Settings.Data.useDials);

	ui.hS_hysterisis->setValue(Settings.Data.fan_hysterisis);

	this->resize(Settings.Data.winSizeX, Settings.Data.winSizeY);

	if (Settings.Data.waitfor)
	{
		for (int i = 0; i < 30; i++)
		{
			if (HWInfo.init(Settings, ui.lW_Status, AutoPages))
			{
				ui.rB_Automode->setChecked(true);
				break;
			}
			QThread::sleep(1);
		}
	}
	else {
		if (HWInfo.init(Settings, ui.lW_Status, AutoPages))
			ui.rB_Automode->setChecked(true);
	}

	timer.setInterval(1000);
	timer.start();

	//reply = manager.get(QNetworkRequest(QString("https://raw.githubusercontent.com/Alia5/FanControllerV2/master/Win32/Release/version")));
	//connect(&manager, SIGNAL(finished(QNetworkReply*)), this,
	//	SLOT(downloadFinished(QNetworkReply*)));
}


void FanControllerV2::initTempGraphs(int Num)
{
	graphTemp[Num].clear();
	graphTime[Num].clear();
	for (int i = 0; i < 101; i++)
	{
		graphTemp[Num].append(0);
		graphTime[Num].append(100);
	}
	AutoPages.apAutoPage[Num].CustomPlot->graph(0)->setData(graphTime[Num], graphTemp[Num]);
}

void FanControllerV2::updateTempGraphs()
{
	for (int j = 0; j < 6; j++)
	{
		if (TempString[j] != AutoPages.apAutoPage[j].ComboBox->currentText())
		{
			initTempGraphs(j);
			TempString[j] = AutoPages.apAutoPage[j].ComboBox->currentText();
		}


		int Temp;

		if (AutoPages.apAutoPage[j].ComboBox->currentIndex() != -1)
		{
			int k;
			for (k = 0; k < ui.lW_Status->count() - 1; k++)
			{
				if (ui.lW_Status->item(k)->text().contains(TempString[j] + ":"))
					break;
			}

			Temp = ui.lW_Status->item(k)->text().remove(QRegExp("[^0-9\\d\\s]")).remove(QRegExp("\\ (.*)\\ ")).remove(" ").toInt();
		}
		else Temp = 0;


		for (int i = 0; i < 100; i++)
		{
			graphTemp[j][i] = graphTemp[j][i + 1];
			graphTime[j][i] = graphTime[j][i + 1] - 1;
			if (i == 0 && j == 0)
			{
				int time[101];
				for (int k = 0; k < 101; k++)
				{
					time[k] = graphTime[j][k];
				}
				int bla = 0;
			}
		}

		graphTemp[j][100] = Temp;
		AutoPages.apAutoPage[j].CustomPlot->graph(0)->setData(graphTime[j], graphTemp[j]);
		AutoPages.apAutoPage[j].CustomPlot->replot();
		TempString[j] = AutoPages.apAutoPage[j].ComboBox->currentText();
	}
}

void FanControllerV2::closeEvent(QCloseEvent *event)
{
	if (trayIcon.isVisible()) {
		updatesettings(); //we write settins everytime program gets minimized to tray... justin case
		hide();
		event->ignore();
	}
}

void FanControllerV2::programClose()
{
	trayIcon.setVisible(false); //enable window closing
	close();
}

void FanControllerV2::restoreSize()
{
	this->showNormal();
	this->resize(750, 400);
}

void FanControllerV2::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::DoubleClick:
		//WIndow show
		showNormal();
		break;
	default:
		;
	}
}

void FanControllerV2::on_hS_Transparency_valueChanged()
{
	setWindowOpacity(qreal(ui.hS_Transparency->value() / 100.f));
}

void FanControllerV2::AutoGraphUpdate(QMouseEvent *e)
{
	int graphnum;
	for (graphnum = 0; graphnum < 6; graphnum++)
	{
		if (AutoPages.apAutoPage[graphnum].CustomPlot == sender())
			break;
		if (graphnum == 5 && AutoPages.apAutoPage[graphnum].CustomPlot != sender())
			return;
	}

	if (e->buttons() == Qt::LeftButton)
	{
		QPointF mousepos;			//mouseccords releative to axis, in axisvalues!
		mousepos.setX(AutoPages.apAutoPage[graphnum].CustomPlot->xAxis->pixelToCoord(e->localPos().x()));		//map coords to values
		mousepos.setY(AutoPages.apAutoPage[graphnum].CustomPlot->yAxis->pixelToCoord(e->localPos().y()));

		if (/*e->type() == QEvent::MouseButtonPress &&*/ mousepos.x() >= -5 && mousepos.x() <= 105 &&
			mousepos.y() >= -5 && mousepos.y() <= 105)
		{
			double xkey = fmod(mousepos.x(), 1);
			xkey = mousepos.x() - xkey;
			double value = mousepos.y();

			if (xkey < 5)
				xkey = 0;

			if (xkey > 100)
				xkey = 100;

			if (value < 0)
				value = 0;
			if (value > 100)
				value = 100;

			if (value != 0 && value < 2)
				value = 1;

			QMap<double, double>::const_iterator lowerKey;
			QMap<double, double>::const_iterator upperKey;

			if (e->type() == QEvent::MouseButtonPress)
			{
				AutoPages.apAutoPage[graphnum].selectedKey = xkey;
			}

			if (xkey == 0)
				lowerKey = AutoPages.apAutoPage[graphnum].Data.lowerBound(0);
			else
				lowerKey = AutoPages.apAutoPage[graphnum].Data.lowerBound(xkey) - 1;

			upperKey = AutoPages.apAutoPage[graphnum].Data.lowerBound(xkey);

			if (AutoPages.apAutoPage[graphnum].Data.contains(xkey))
				upperKey += 1;

			if (xkey != 0 && value < lowerKey.value())
				value = lowerKey.value();
			if (value > upperKey.value())
				value = upperKey.value();

			if (xkey > 0 && value == lowerKey.value() && value == upperKey.value())
				return;
			if (upperKey == AutoPages.apAutoPage[graphnum].Data.end())
				return;
			if (value == 0 && lowerKey != AutoPages.apAutoPage[graphnum].Data.begin())
				return;

			if (e->type() == QEvent::MouseButtonPress)
			{
				//if (mousepos.x() - xkey >= 2.5f)
				//	xkey += 5;
				AutoPages.apAutoPage[graphnum].Data.insert(xkey, value);
			}
			else {
				AutoPages.apAutoPage[graphnum].Data.remove(AutoPages.apAutoPage[graphnum].selectedKey);

				AutoPages.apAutoPage[graphnum].Data.insert(xkey, value);
				AutoPages.apAutoPage[graphnum].selectedKey = xkey;
			}

			if (!AutoPages.apAutoPage[graphnum].Data.contains(0))
				AutoPages.apAutoPage[graphnum].Data.insert(0, value);

			AutoPages.apAutoPage[graphnum].Data.insert(100, 100);			//for safety lets force the user to have 100% at 100°C a bit high... bit just to make a point more or less

			QToolTip::showText(e->globalPos(), QString::number(xkey) + QString::fromStdWString(L"°C, ") + QString::number(value, 'g', 3) + "%", this, rect());

			AutoPages.apAutoPage[graphnum].CustomPlot->graph(1)->setData(AutoPages.apAutoPage[graphnum].Data.keys().toVector(), AutoPages.apAutoPage[graphnum].Data.values().toVector());
			AutoPages.apAutoPage[graphnum].CustomPlot->replot();
		}
	}
}

void FanControllerV2::update()
{
	timer.stop();

	//check if we should show our window from sharedmemory
	Instance.lock();
	QString MemString = QString((char*)Instance.data());
	if (MemString == "ShowWindow")
	{
		this->showNormal();
		this->activateWindow();
		this->raise();
		memset(Instance.data(), NULL, 1024);
	}
	Instance.unlock();

	//gettemps
	if (!HWInfo.update())
	{
		if (this->isHidden())
			showMinimized();
		if (ui.rB_Automode->isChecked())
		{
			for (int i = 0; i < 6; i++)
			{
				StatusPage.fsFanStatus[i].Fanslider->setValue(255);
				calibrationHelp.shouldCalibrate[i] = false;
			}
		}
		if (ui.rB_Automode->isChecked())
			wasAuto = true;
		else if (ui.rB_Automode->isEnabled())
			wasAuto = false;

		ui.rB_Manualmode->setChecked(true);
		ui.rB_Automode->setEnabled(false);
		ui.pB_freeze->setEnabled(false);
		ui.pB_silent->setEnabled(false);
	}
	else 
	{
		if (ui.rB_Automode->isChecked())
			wasAuto = true;
		else if (ui.rB_Automode->isEnabled())
			wasAuto = false;
		ui.rB_Automode->setEnabled(true);
		if (wasAuto)
			ui.rB_Automode->setChecked(true);

		ui.pB_freeze->setEnabled(true);
		ui.pB_silent->setEnabled(true);

		//update graphs
		updateTempGraphs();

	}


	//calculate fanvaluesvalues
	if (!silent && !freeze)
	{
		for (int i = 0; i < 6; i++)
		{
			if (!calibrationHelp.shouldCalibrate[i])
			{
				// AUTOSETTINGS
				if (ui.rB_Automode->isChecked())
				{
					for (int j = 0; j < ui.hS_hysterisis->value() - 1; j++)
					{
						temps[i][j] = temps[i][j + 1];
					}

					if (AutoPages.apAutoPage[i].ComboBox->currentIndex() != -1)
					{
						int k;
						for (k = 0; k < ui.lW_Status->count() - 1; k++)
						{
							if (ui.lW_Status->item(k)->text().contains(AutoPages.apAutoPage[i].ComboBox->currentText() + ":"))
								break;
						}

						temps[i][ui.hS_hysterisis->value() - 1] = ui.lW_Status->item(k)->text().remove(QRegExp("[^0-9\\d\\s]")).remove(QRegExp("\\ (.*)\\ ")).remove(" ").toInt();

						QCPItemTracer tracer(AutoPages.apAutoPage[i].CustomPlot);
						tracer.setGraph(AutoPages.apAutoPage[i].CustomPlot->graph(1));

						float temp = 0;
						for (int j = 0; j < ui.hS_hysterisis->value(); j++)
						{
							temp += temps[i][j];
						}

						tracer.setGraphKey(temp / ui.hS_hysterisis->value());

						tracer.setInterpolating(true);
						tracer.updatePosition();
						int val = tracer.position->value() / 100.f * StatusPage.fsFanStatus[i].Fanslider->maximum();
						StatusPage.fsFanStatus[i].Fanslider->setValue(val);
					}
				}

				// Keep fan off
				if (StatusPage.fsFanStatus[i].KeepOff->isChecked())
					StatusPage.fsFanStatus[i].Fanslider->setValue(0);

				// MAX TEMP SETTINGS
				if (ui.lW_Status->count() > 0)
				{
					if (ui.lW_Status->item(Settings.Data.indexOfCPUTemp)->text().remove(QRegExp("[^0-9\\d\\s]")).toInt() >= Settings.Data.maxCPUTemp)
						StatusPage.fsFanStatus[i].Fanslider->setValue(StatusPage.fsFanStatus[i].Fanslider->maximum());
				}
			}
		}
	} 
	else
	{
		if (silent)
		{
			for (int i = 0; i < 6; i++)
			{
				if (ui.lW_Status->item(Settings.Data.indexOfCPUTemp)->text().remove(QRegExp("[^0-9\\d\\s]")).toInt() >= 65)
				{
					silent = false;
					ui.pB_silent->setText("Silence");
					ui.pB_silent->setStyleSheet("");
				}
				if (!calibrationHelp.shouldCalibrate[i])
				{
					QCPItemTracer tracer(AutoPages.apAutoPage[i].CustomPlot);
					tracer.setGraph(AutoPages.apAutoPage[i].CustomPlot->graph(1));
					tracer.setGraphKey(30);
					tracer.setInterpolating(true);
					tracer.updatePosition();
					int val = tracer.position->value() / 100.f * StatusPage.fsFanStatus[i].Fanslider->maximum();
					StatusPage.fsFanStatus[i].Fanslider->setValue(val);
				}
			}
		}
		if (freeze)
		{
			if (ui.lW_Status->item(Settings.Data.indexOfCPUTemp)->text().remove(QRegExp("[^0-9\\d\\s]")).toInt() < 40)
				stopFreeze();
			for (int i = 0; i < 6; i++)
			{
				if (!calibrationHelp.shouldCalibrate[i])
				{
					StatusPage.fsFanStatus[i].Fanslider->setValue(StatusPage.fsFanStatus[i].Fanslider->maximum());
				}
			}
		}
	}

	//communicate with device
	for (int i = 0; i < 6; i++)
	{
		if (StatusPage.fsFanStatus[i].Fanslider->value() == 0)
			fanvalues.FanValues[i] = 0;
		else
			fanvalues.FanValues[i] = StatusPage.fsFanStatus[i].Fanslider->value() + (255 - StatusPage.fsFanStatus[i].Fanslider->maximum());
	}

	if (!device.update(fanvalues))
	{
		for (int i = 0; i < 6; i++)
		{
			StatusPage.fsFanStatus[i].RPM->setText("??? RPM");
		}
		if (this->isHidden())
			showMinimized();
		timer.start();
		return;
	}

	//update rpm values
	for (int i = 0; i < 6; i++)
	{
		StatusPage.fsFanStatus[i].RPM->setText(QString::fromStdString(std::to_string(fanvalues.RPMValues[i])) + " RPM");
	}

	timer.start();
}

void FanControllerV2::SliderValChanged(int value)
{
	int slidernum;
	for (slidernum = 0; slidernum < 6; slidernum++)
	{
		if (StatusPage.fsFanStatus[slidernum].Fanslider == sender())
			break;
		if (slidernum == 5 && StatusPage.fsFanStatus[slidernum].Fanslider != sender())
			return;
	}
	StatusPage.fsFanStatus[slidernum].Percentage->setText(QString::number((value / (StatusPage.fsFanStatus[slidernum].Fanslider->maximum() / 100.f))).remove(4, 255).append("%"));
	StatusPage.fsFanStatus[slidernum].Voltage->setText("~" + QString::number((value + (255 - StatusPage.fsFanStatus[slidernum].Fanslider->maximum())) / 21.25, 'g', 2).append("V"));
	if (StatusPage.fsFanStatus[slidernum].Percentage->text() == "0%")
		StatusPage.fsFanStatus[slidernum].Voltage->setText("0.0V");
}

void FanControllerV2::updatesettings()
{
	Settings.Data.autoRun = ui.cb_autorun->isChecked();
	Settings.Data.startMini = ui.cb_startmini->isChecked();
	Settings.Data.waitfor = ui.cb_waitfor->isChecked();

	if (Settings.Data.useAIDA != ui.rB_Aida->isChecked())
	{
		for (int i = 0; i < 6; i++)
		{
			AutoPages.apAutoPage[i].Item = "UNINIT";
		}
		QMessageBox::warning(this, "FanController", "This change resets the \"React to:\" value from \"Automatic settings\"!");
	}

	Settings.Data.useAIDA = ui.rB_Aida->isChecked();
	Settings.Data.maxCPUTemp = ui.sB_MaxTemp->value();
	Settings.Data.useSliders = ui.cb_sliders->isChecked();
	Settings.Data.useDials = ui.cb_dials->isChecked();
	Settings.Data.winSizeX = this->size().width();
	Settings.Data.winSizeY = this->size().height();

	Settings.Data.AutoPages = this->AutoPages;

	Settings.Data.fan_hysterisis = ui.hS_hysterisis->value();

	Settings.WriteSettings();
}


void FanControllerV2::updateAutostart()
{
	//autorun win32...
#ifdef _WIN32
	QSettings RegSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		QSettings::NativeFormat);
	if (ui.cb_autorun->isChecked())
	{
		RegSettings.setValue("FanControlV2", "\"" + QDir::toNativeSeparators(
			QCoreApplication::applicationFilePath()) + "\"");
	}
	else {
		RegSettings.remove("FanControlV2");
	}
#endif
}


void FanControllerV2::on_tW_Tabs_tabBarClicked(int n)
{
	Device::deviceinfo info = device.getDeviceInfo();
	ui.l_Manufact->setText("Manufacturer: " + info.Manufacturer);
	ui.l_Product->setText("Product Name: " + info.Product);
	ui.l_Serial->setText("SerialNumber: " + info.Serial);
}

void FanControllerV2::startCalibration()
{
	if (ui.pB_FanCaliAll == sender())
	{
		for (int i = 0; i < 6; i++)
		{
			StatusPage.fsFanStatus[i].Fanslider->setMaximum(255);
			calibrationHelp.shouldCalibrate[i] = true;
			calibrationHelp.togglefull[i] = 0;
			calibrationHelp.value[i] = 0;
			calibrationHelp.waitsec[i] = true;
		}
	}
	else
	{
		StatusPage.fsFanStatus[ui.sB_Rst_FanNum->value() - 1].Fanslider->setMaximum(255);
		calibrationHelp.shouldCalibrate[ui.sB_Cali_FanNum->value() - 1] = true;
		calibrationHelp.togglefull[ui.sB_Cali_FanNum->value() - 1] = 0;
		calibrationHelp.value[ui.sB_Cali_FanNum->value() - 1] = 0;
		calibrationHelp.waitsec[ui.sB_Cali_FanNum->value() - 1] = true;
	}

	ui.pB_FanCali->setEnabled(false);
	ui.pB_FanCaliAll->setEnabled(false);
	ui.pB_FanRst->setEnabled(false);
	ui.pB_FanRstAll->setEnabled(false);

	QMessageBox::information(this, "FanControll", "Fan(s) can stop spinning for up to 60 seconds!\nAfter calibration, please check if fans are really spinning.");

	QTimer::singleShot(0, this, SLOT(calibrateFan()));
}
void FanControllerV2::resetCalibration()
{
	if (ui.pB_FanRstAll == sender())
	{
		for (int i = 0; i < 6; i++)
		{
			StatusPage.fsFanStatus[i].Fanslider->setMaximum(255);
			StatusPage.fsFanStatus[i].Fanslider->setValue(255);
			Settings.Data.CalibrationData[i].calibrated = false;
			return;
		}
	}

	StatusPage.fsFanStatus[ui.sB_Rst_FanNum->value() - 1].Fanslider->setMaximum(255);
	StatusPage.fsFanStatus[ui.sB_Rst_FanNum->value() - 1].Fanslider->setValue(255);
	Settings.Data.CalibrationData[ui.sB_Rst_FanNum->value() - 1].calibrated = false;
	updatesettings();
}

void FanControllerV2::manualCalibration()
{
	Settings.Data.CalibrationData[ui.sB_ManCali_FanNum->value() - 1].value = StatusPage.fsFanStatus[ui.sB_ManCali_FanNum->value() - 1].Fanslider->value();
	StatusPage.fsFanStatus[ui.sB_ManCali_FanNum->value() - 1].Fanslider->setMaximum(255 - StatusPage.fsFanStatus[ui.sB_ManCali_FanNum->value() - 1].Fanslider->value());
	StatusPage.fsFanStatus[ui.sB_ManCali_FanNum->value() - 1].Fanslider->setValue(1);
	StatusPage.fsFanStatus[ui.sB_ManCali_FanNum->value() - 1].Fanslider->setEnabled(true);
	Settings.Data.CalibrationData[ui.sB_ManCali_FanNum->value() - 1].calibrated = true;
	calibrationHelp.shouldCalibrate[ui.sB_ManCali_FanNum->value() - 1] = false;
	updatesettings();
}

void FanControllerV2::calibrateFan()
{
	timer.stop();

	bool allfalse = true;
	for (int i = 0; i < 6; i++)
	{
		if (calibrationHelp.shouldCalibrate[i])
		{
			allfalse = false;
			break;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (calibrationHelp.shouldCalibrate[i])
		{
			StatusPage.fsFanStatus[i].Fanslider->setEnabled(false);		//prevent user interactions
			int rpm = StatusPage.fsFanStatus[i].RPM->text().remove(" RPM").toInt();
			//spin the fan up initially to check if there is a fan connected, then let the fan stop and wait additional 30 seconds, just to be safe!
			switch (calibrationHelp.togglefull[i])
			{
			case -1:
				break;
			case 0:
				calibrationHelp.value[i] = 255;
				StatusPage.fsFanStatus[i].Fanslider->setValue(calibrationHelp.value[i]);
				calibrationHelp.togglefull[i]++;
				break;
			case 3:
				if (rpm < 30)
				{
					QMessageBox::warning(this, "FanControll", "Error! Check Fan-Connection: " + QString::number(i+1));
					calibrationHelp.shouldCalibrate[i] = false;
					StatusPage.fsFanStatus[i].Fanslider->setEnabled(true);
				}
				calibrationHelp.togglefull[i]++;
				break;
			case 4:
				calibrationHelp.value[i] = 0;
				StatusPage.fsFanStatus[i].Fanslider->setValue(calibrationHelp.value[i]);
				calibrationHelp.togglefull[i]++;
				break;
			case 5:
				if (rpm == 0)
				{
					calibrationHelp.togglefull[i]++;
				}
				break;
			case 25:
				calibrationHelp.togglefull[i] = -1;
				break;
			default:
				calibrationHelp.togglefull[i]++;
			}

			//slowly ramp the fan up, and check if it started spinning
			if (calibrationHelp.togglefull[i] == -1)
			{
				if (rpm < 60)
				{
					if (calibrationHelp.value[i] >= 250)
					{
						StatusPage.fsFanStatus[i].Fanslider->setValue(0);
						calibrationHelp.shouldCalibrate[i] = false;
						StatusPage.fsFanStatus[i].Fanslider->setEnabled(true);
						QMessageBox::warning(this, "FanControll", "Error! Check Fan-Connection: " + QString::number(i + 1));
					}
					else
					{
						calibrationHelp.value[i] += 10;
						StatusPage.fsFanStatus[i].Fanslider->setValue(calibrationHelp.value[i]);
					}
				}
				else
				{
					//as a safety feature, if the fan reports some rpm.. we wait a second and see if it still reports a rpm value
					if (!calibrationHelp.waitsec[i])
					{
						Settings.Data.CalibrationData[i].value = StatusPage.fsFanStatus[i].Fanslider->value();
						StatusPage.fsFanStatus[i].Fanslider->setMaximum(255 - StatusPage.fsFanStatus[i].Fanslider->value());
						StatusPage.fsFanStatus[i].Fanslider->setValue(1);
						StatusPage.fsFanStatus[i].Fanslider->setEnabled(true);
						Settings.Data.CalibrationData[i].calibrated = true;
						calibrationHelp.shouldCalibrate[i] = false;
						trayIcon.showMessage("FanControll", "Calibration for Fan " + QString::number(i + 1) + " successful", QSystemTrayIcon::Information, 5000);
						updatesettings();
					}
					else
					{
						calibrationHelp.waitsec[i] = false;
					}
				}
			}
		}
	}

	update();
	timer.stop();

	if (allfalse)
	{
		timer.start();
		ui.pB_FanCali->setEnabled(true);
		ui.pB_FanCaliAll->setEnabled(true);
		ui.pB_FanRst->setEnabled(true);
		ui.pB_FanRstAll->setEnabled(true);
	}

	else
		QTimer::singleShot(1000, this, SLOT(calibrateFan()));
}

void FanControllerV2::on_pB_silent_clicked()
{
	if (silent)
	{
		stopFreeze();
		silent = false;
		ui.pB_silent->setStyleSheet("");
		return;
	}

	stopFreeze();
	freezeTimer.stop();
	silent = true;
	ui.pB_freeze->setText("Freeze");
	ui.pB_silent->setText("Silencing!");
	ui.pB_silent->setStyleSheet("color: rgb(255, 0, 0)");
	ui.pB_freeze->setStyleSheet("");
}


void FanControllerV2::on_pB_freeze_clicked()
{
	if (freeze)
	{
		stopFreeze();
		silent = false;
		return;
	}
	silent = false;
	freeze = true;
	ui.pB_freeze->setText("Freezing!");
	ui.pB_freeze->setStyleSheet("color: rgb(0, 0, 255)");
	ui.pB_silent->setStyleSheet("");
	ui.pB_silent->setText("Silence");
	freezeTimer.singleShot(60000, this, SLOT(stopFreeze()));
	//QTimer::singleShot(60000, this, SLOT(stopFreeze()));
}

void FanControllerV2::stopFreeze()
{
	freezeTimer.stop();
	freeze = false;
	ui.pB_freeze->setStyleSheet("");
	ui.pB_silent->setStyleSheet("");
	ui.pB_freeze->setText("Freeze");
	ui.pB_silent->setText("Silence");
}

void FanControllerV2::downloadFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	if (reply->error()) {
		QMessageBox::warning(this, "FanControll", "Couldn't check for updates! \n" + reply->errorString());
	}
	else {
		int versionnumber = QString((QString)reply->readAll()).toInt();
		if (versionnumber > Settings.Data.versionnumber)
			QMessageBox::information(this, "FanControll", "Update availible! <a href='http://raw.githubusercontent.com/Alia5/FanControllerV2/master/Win32/Release/Fancontroll-installer.exe'>Download Here</a><br />"
				"Update Version: " + QString::number(float(versionnumber / 100.f), 'g', 3) + "<br />"
				"You arer currently running Version: " + QString::number(float(Settings.Data.versionnumber / 100.f), 'g', 3));
	}
	reply->deleteLater();
}

void FanControllerV2::about()
{
	QMessageBox msgBox(this);
	msgBox.setWindowTitle("Help / About");
	msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
	msgBox.setText("This FanController project  was created by my desire to to have a custom FanController-Hardware, which has 6 channels,<br />"
		"is usb - controlled, and gets its temperatues from the hardware - sensors instead of some crappy temperature fingers you attach to a heatsink<br />"
		"this is the controll application for my usb fan controller.<br />"
		"You can find the hardware-part <a href='https://github.com/Alia5/atmega328p_6_channel_usb_fancontroller'>here</a><br />"
		"<br / >"
		"if you got this application from the original author, you most likeley know everything about this there is to know<br />"
		"else, if you have any questions you can <a href='https://github.com/Alia5/'>contact me</a> for any help.<br />"
		"This project is Licensed under GPLv3<br /><br />"
		"Third Party Licences:<br />"
		"Qt: LGPLv3<br />"
		"HIDAPI: GPLv3<br />"
		"QCustomPlot: GPLv3<br /><br />"
		"You can find more information about the licenses in the application-directory in the corresponding files<br /><br />" 
		"Version: " + QString::number(float(Settings.Data.versionnumber / 100.f), 'g', 3));
	msgBox.exec();
}

FanControllerV2::~FanControllerV2()
{
	trayIcon.setVisible(false);
	Settings.Data.autoRun = ui.cb_autorun->isChecked();
	Settings.Data.startMini = ui.cb_startmini->isChecked();
	Settings.Data.useAIDA = ui.rB_Aida->isChecked();
	Settings.Data.maxCPUTemp = ui.sB_MaxTemp->value();
	Settings.Data.useSliders = ui.cb_sliders->isChecked();
	Settings.Data.useDials = ui.cb_dials->isChecked();
	Settings.Data.winSizeX = this->size().width();
	Settings.Data.winSizeY = this->size().height();

	Settings.Data.AutoPages = this->AutoPages;

	Settings.Data.fan_hysterisis = ui.hS_hysterisis->value();
}