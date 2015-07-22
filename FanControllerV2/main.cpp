#include "fancontrollerv2.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{

	QSharedMemory InstanceTest("Alia5-FanController");
	if (!InstanceTest.create(1024) && InstanceTest.error() == QSharedMemory::AlreadyExists)
	{
		InstanceTest.attach(QSharedMemory::ReadWrite);
		char* write = "ShowWindow";
		InstanceTest.lock();
		memcpy(InstanceTest.data(), write, 11);
		InstanceTest.unlock();
		return 1;
	}

	QApplication a(argc, argv);
	FanControllerV2 w;
	w.show();
	return a.exec();
}