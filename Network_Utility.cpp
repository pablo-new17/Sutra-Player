#include <QDebug>
#include <QDebugStateSaver>
#include <QNetworkInterface>

#include "Network_Utility.h"


QDebug operator<<(QDebug d, const Network_Device& type)
{
	QDebugStateSaver saver(d);
	d.nospace().noquote()
			<< "( Device_Name:" << type.Device_Name << ", "
			<< "MAC_Address:" << type.MAC_Address << ", "
			<< "Flags :" << type.Flags << ", "
			<< "is Virual:" << type.Virual << ", "
			<< "IP_Address:" << type.IP_Address << ", "
			<< "Mask:" << type.Netmask << ", "
			<< "Broadcast:" << type.Broadcast << ")";
	return d;
}


Network_Device::Network_Device()
{
	this->Virual = false;
}

Network_Device::Network_Device(QString Device_Name)
{
	this->Virual = false;
	this->Device_Name = Device_Name;
}

Network_Device::Network_Device(QString Device_Name, QString IP_Address, QString Mask)
{
	this->Virual = false;
	this->Device_Name = Device_Name;
	this->IP_Address = IP_Address;
	this->Netmask = Mask;
}


Network_Utility::Network_Utility(QObject *parent) : QObject(parent)
{
	foreach(QNetworkInterface Net_Device, QNetworkInterface::allInterfaces())
	{
		if(!Net_Device.isValid())					continue;
		if(Net_Device.type() != QNetworkInterface::Ethernet &&
		   Net_Device.type() != QNetworkInterface::Wifi)		continue;
		if(!Net_Device.flags().testFlag(QNetworkInterface::IsUp) ||
		   !Net_Device.flags().testFlag(QNetworkInterface::IsRunning))	continue;



		Network_Device New_Device(Net_Device.humanReadableName());
		New_Device.MAC_Address = Net_Device.hardwareAddress();
		New_Device.Flags = Net_Device.flags();

		if(New_Device.Device_Name.contains("VirtualBox") ||
		   New_Device.Device_Name.contains("VMware"))
			New_Device.Virual = true;

		foreach (QNetworkAddressEntry Address, Net_Device.addressEntries())
		{
			if(Address.ip().protocol() != QAbstractSocket::IPv4Protocol)	continue;

			New_Device.IP_Address = Address.ip().toString();
			New_Device.Netmask = Address.netmask().toString();
			New_Device.Broadcast = Address.broadcast().toString();
		}

		this->m_All_Devices.append(New_Device);
		qDebug() << New_Device.Device_Name << New_Device.IP_Address;
//		qDebug() << New_Device;
	}

}

QList<Network_Device> Network_Utility::List_Network_Device()
{
	QList<Network_Device> IP_Address;

	foreach(Network_Device Net_Device, this->m_All_Devices)
	{
		if(Net_Device.Virual) continue;

		IP_Address.append(Net_Device);
	}

	return IP_Address;
}

QList<Network_Device> Network_Utility::List_Network_Device_Virtual()
{
	return this->m_All_Devices;
}


