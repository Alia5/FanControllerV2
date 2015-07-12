#ifndef FANCONTROLLERV2_H
#define FANCONTROLLERV2_H

#include <QtWidgets/QMainWindow>
#include "ui_fancontrollerv2.h"

#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QtGui>
#include <QMenu>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QTimer>
#include <QSettings>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "Device.h"
#include "ui_fan.h"
#include "SettingsHandler.h"
#include "HWInfo.h"


class FanControllerV2 : public QMainWindow
{
	Q_OBJECT

public:
	FanControllerV2(QWidget *parent = 0);
	~FanControllerV2();

private:
	//qt generated
	Ui::FanControllerV2Class ui;

	QSystemTrayIcon trayIcon;
	QIcon ticon;
	QMenu trayIconMenu;

	QMessageBox msgbox;

	QTimer timer;

	void init();

	Device device;

	Device::UpdateValues fanvalues;
	ui_fan::FanStatusPage StatusPage;
	ui_fan::AutoPages	AutoPages;

	SettingsHandler Settings;
	HWInfo HWInfo;

	void initGraphs();
	void updateGraphs();

	QVector<double> graphCpuTemp, graphTime;

	QNetworkAccessManager manager;
	QNetworkReply *reply;

	static struct FcalibrationHelp
	{
		bool shouldCalibrate[6];
		int togglefull[6];
		int value[6];
		bool waitsec[6];
	};
	FcalibrationHelp calibrationHelp;

	bool wasAuto = true;
	int temps[6][5];

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void programClose();
	void restoreSize();
	void on_hS_Transparency_valueChanged();
	void SliderValChanged(int value);

	void AutoGraphUpdate(QMouseEvent *e);

	void update();

	void updatesettings();

	void on_tW_Tabs_tabBarClicked(int n);

	void startCalibration();
	void resetCalibration();
	void manualCalibration();
	void calibrateFan();

	void downloadFinished(QNetworkReply *reply);

	void about();

public:
	QSystemTrayIcon getTrayIcon()
	{
		return &trayIcon;
	}

};

#endif // FANCONTROLLERV2_H
