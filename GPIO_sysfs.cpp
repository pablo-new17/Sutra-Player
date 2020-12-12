#include <QDebug>
#include <QSocketNotifier>
#include <QTime>
#include <QThread>

#include "GPIO_sysfs.h"

#define TIME_INTERVAL	100	//ms

int toBCM[] = {-1, -1, 2, -1, 3, -1, 4, 14, -1, 15, 17, 18, 27, -1, 22, 23, -1, 24, 10, -1, 9, 25, 11, 8, -1, 7, 0, 1, 5, -1, 6, 12, 13, -1, 19, 16, 26, 20, -1, 21};

GPIO_sysfs::GPIO_sysfs(int GPIO_Pin_Number, GPIO_DIRECTION Direction, QObject *parent) : QObject(parent)
{
	this->m_GPIO_Number = GPIO_Pin_Number;
	this->m_Direction = Direction;

	this->init();
}

GPIO_sysfs::~GPIO_sysfs()
{
	QFile exportFile("/sys/class/gpio/unexport");
	exportFile.open(QIODevice::WriteOnly);
	exportFile.write(QString("%1").arg(this->m_Pin).toUtf8());
}

bool GPIO_sysfs::Set_Value(bool High)
{
	if(this->m_Direction==GPIO_DIRECTION_IN)	return false;

	QFile valueFile(this->m_Value);
	valueFile.open(QIODevice::WriteOnly);

	valueFile.write(High?"1":"0");

	valueFile.close();

	return true;
}

bool GPIO_sysfs::Get_Value()
{
	if(this->m_Direction==GPIO_DIRECTION_OUT)	return false;

	QFile valueFile(this->m_Value);
	valueFile.open(QIODevice::ReadOnly);

	QString Value(valueFile.readAll());

	valueFile.close();

	qDebug() << Value;

	return Value.toInt();
}

bool GPIO_sysfs::init()
{
	this->m_Pin = toBCM[this->m_GPIO_Number-1];

	qDebug() << "GPIO pin number" << this->m_Pin;
	if(this->m_Pin == -1)	return false;

	QFile exportFile("/sys/class/gpio/export");
	exportFile.open(QIODevice::WriteOnly);
	exportFile.write(QString("%1").arg(this->m_Pin).toUtf8());
	exportFile.close();

	QFile directionFile(QString("/sys/class/gpio/gpio%1/direction").arg(this->m_Pin));
	directionFile.open(QIODevice::WriteOnly);
	directionFile.write(this->m_Direction==GPIO_DIRECTION_OUT?"out":"in");
	directionFile.close();

	this->m_Value = QString("/sys/class/gpio/gpio%1/value").arg(this->m_Pin);
	if(this->m_Direction==GPIO_DIRECTION_IN)
	{
		QFile edgeFile(QString("/sys/class/gpio/gpio%1/edge").arg(this->m_Pin));
		edgeFile.open(QIODevice::WriteOnly);
		edgeFile.write("both");
		edgeFile.close();

		qDebug() << "watch" << QString(this->m_Value);

		this->m_Watcher.addPath(this->m_Value);
		connect(&this->m_Watcher, &QFileSystemWatcher::fileChanged, this, &GPIO_sysfs::activated);
	}

	return true;
}

void GPIO_sysfs::activated()
{
	static QTime Last_Event = QTime::currentTime();

	if(Last_Event.msecsTo(QTime::currentTime()) < TIME_INTERVAL)
	{
		this->Get_Value();
		return;
	}
	Last_Event = QTime::currentTime();

	QThread::msleep(TIME_INTERVAL);
	qDebug() << "pin value changed" << this->Get_Value();
	emit this->Input_Value_Changed();
}
