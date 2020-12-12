#include <QCoreApplication>
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QThread>

#include "MainWindow.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	MainWindow w;
	w.show();

	return a.exec();
}
