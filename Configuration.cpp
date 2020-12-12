#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>

#include "Configuration.h"

Configuration::Configuration(QString Top_Folder, QObject *parent) : QObject(parent)
{
	this->m_Top_Folder = Top_Folder;

	QString Config_File = Find_Config_File();

	this->m_Config = new QSettings(Config_File, QSettings::IniFormat);

	if(!this->m_Config->childGroups().contains("SUTRA"))
	{
		QString ErrorMessage = QString("No sutra play settings, so create it at %1")
				       .arg(Config_File);
		qWarning() << ErrorMessage;

		this->Create_Config();
	}
}


//QMap<QString, QVariant> Configuration::getDatabase() const
//{
//	QMap<QString, QVariant> Data;

//	this->m_Config->beginGroup("DATABASE");
//	foreach(QString Key, this->m_Config->childKeys())
//	{
//		Data.insert(Key, this->m_Config->value(Key));
//	}
//	this->m_Config->endGroup();

//	return Data;
//}

QString Configuration::getDatabase_Server() const
{
	QString Data;

	this->m_Config->beginGroup("DATABASE");
		Data = this->m_Config->value("Server").toString();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setDatabase_Server(const QString Server)
{
	this->m_Config->beginGroup("DATABASE");
		this->m_Config->setValue("Server", Server);
	this->m_Config->endGroup();
}

int Configuration::getDatabase_Port() const
{
	int Data;

	this->m_Config->beginGroup("DATABASE");
		Data = this->m_Config->value("Port").toInt();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setDatabase_Port(const int Port)
{
	this->m_Config->beginGroup("DATABASE");
		this->m_Config->setValue("Port", Port);
	this->m_Config->endGroup();
}

QString Configuration::getDatabase_User() const
{
	QString Data;

	this->m_Config->beginGroup("DATABASE");
		Data = this->m_Config->value("User").toString();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setDatabase_User(const QString User)
{
	this->m_Config->beginGroup("DATABASE");
		this->m_Config->setValue("User", User);
	this->m_Config->endGroup();
}

QString Configuration::getDatabase_Pass() const
{
	QString Data;

	this->m_Config->beginGroup("DATABASE");
		Data = this->m_Config->value("Pass").toString();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setDatabase_Pass(const QString Pass)
{
	this->m_Config->beginGroup("DATABASE");
		this->m_Config->setValue("Pass", Pass);
	this->m_Config->endGroup();
}

QString Configuration::getLocation_Media_Folder() const
{
	QString Data;

	this->m_Config->beginGroup("LOCATION");
		Data = this->m_Config->value("Media_Folder").toString();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setLocation_Media_Folder(const QString Media_Folder)
{
	this->m_Config->beginGroup("LOCATION");
		this->m_Config->setValue("Media_Folder", Media_Folder);
	this->m_Config->endGroup();
}

bool Configuration::getSutra_Created() const
{
	bool Data;

	this->m_Config->beginGroup("SUTRA");
		Data = this->m_Config->value("Created").toBool();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setSutra_Created(const bool Created)
{
	this->m_Config->beginGroup("SUTRA");
		this->m_Config->setValue("Created", Created);
	this->m_Config->endGroup();
}

QString Configuration::getSutra_Current() const
{
	QString Data;

	this->m_Config->beginGroup("SUTRA");
		Data = this->m_Config->value("Current").toString();
	this->m_Config->endGroup();

	return Data;
}

void Configuration::setSutra_Current(const QString Current)
{
	this->m_Config->beginGroup("SUTRA");
		this->m_Config->setValue("Current", Current);
	this->m_Config->endGroup();
}


QSettings* Configuration::getConfig() const
{
	return m_Config;
}


QString Configuration::Find_Config_File()
{
	QString	Filename = "Configuration.ini";
	QString Default = QString("%1/%2").arg(this->m_Top_Folder).arg(Filename);

	QFileInfo* Location = new QFileInfo(Default);
	if(!Location->exists())	Location = new QFileInfo(QDir::homePath() + "/" + Filename);
	if(!Location->exists()) Location = new QFileInfo("/etc/" + QCoreApplication::applicationFilePath() + "/" + Filename);
	if(!Location->exists()) Location = new QFileInfo("/etc/" + Filename);
	if(!Location->exists())	Location = new QFileInfo(Default); // 當以上都不存在，用回 Default

	if(!Location->exists())
	{
		qWarning() << "The configuration file doest not exist";
	}

	return Location->absoluteFilePath();
}

void Configuration::Create_Config()
{
	this->m_Config->beginGroup("DATABASE");
		this->m_Config->setValue("Server", QString("%1/%2").arg(this->m_Top_Folder).arg("Sutra_List.db"));
		this->m_Config->setValue("Port", 3306);
		this->m_Config->setValue("User", "username");
		this->m_Config->setValue("Pass", "password");
	this->m_Config->endGroup();

	this->m_Config->beginGroup("LOCATION");
		this->m_Config->setValue("Media_Folder", QString("%1/media").arg(this->m_Top_Folder));
	this->m_Config->endGroup();

	this->m_Config->beginGroup("SUTRA");
		this->m_Config->setValue("Created", 0);
		this->m_Config->setValue("Current", "");
	this->m_Config->endGroup();
	this->m_Config->sync();
}
