#ifndef NETWORK_UTILITY_H
#define NETWORK_UTILITY_H

#include <QObject>
#include <QDebug>
#include <QNetworkInterface>

class Network_Device
{
public:
	explicit Network_Device();
	explicit Network_Device(QString Device_Name);
	explicit Network_Device(QString Device_Name, QString IP_Address, QString Netmask);

	QString MAC_Address;
	QString Device_Name;
	QString IP_Address;
	QString Netmask;
	QString Broadcast;
	bool	Virual;
	QNetworkInterface::InterfaceFlags Flags;
};

QDebug operator<< (QDebug d, const Network_Device &type);


class Network_Utility : public QObject
{
	Q_OBJECT
public:
	explicit Network_Utility(QObject *parent = nullptr);

	QList<Network_Device> List_Network_Device();
	QList<Network_Device> List_Network_Device_Virtual();
signals:

private:
	QList<Network_Device> m_All_Devices;

};

#endif // NETWORK_UTILITY_H
