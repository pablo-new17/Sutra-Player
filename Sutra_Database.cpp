#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QSqlRecord>

#include "Sutra_Database.h"

Sutra_Database::Sutra_Database(QSettings* Config)
{
	if(QSqlDatabase::isDriverAvailable("QSQLITE")==false)
	{
		QString ErrorMessage = "No SQLLite database engine exist";
		qCritical() << ErrorMessage;
		throw ErrorMessage;
	}

	if(!Config->childGroups().contains("DATABASE"))
	{
		QString ErrorMessage = "No database configuration settings exist";
		qCritical() << ErrorMessage;
		throw ErrorMessage;
	}
	Config->beginGroup("DATABASE");
	this->m_Database_Server = Config->value("Server").toString();
	Config->endGroup();

	this->m_Database = QSqlDatabase::addDatabase("QSQLITE");
	this->m_Database.setDatabaseName(this->m_Database_Server);
	if (!this->m_Database.open())
	{
		QString ErrorMessage = QString("Unabled to open database %1: %2")
							   .arg(this->m_Database_Server)
							   .arg(this->m_Database.lastError().text());
		qCritical() << ErrorMessage;
		//throw ErrorMessage;
	}
}

bool Sutra_Database::Create()
{
	QSqlQuery	Query;

	Query.prepare("CREATE TABLE IF NOT EXISTS `經文` ("
				  "`編號` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
				  "`檔案名稱` TEXT NOT NULL,"
				  "`撥放中` INTEGER NOT NULL )");

	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return false;
	}

	return true;
}
bool Sutra_Database::Clean()
{
	QSqlQuery	Query;

	Query.prepare("DROP TABLE IF EXISTS `經文`");
	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return false;
	}

	return true;
}


bool Sutra_Database::Add(QString Filename)
{
	QSqlQuery	Query;

	Query.prepare("INSERT INTO `經文` (`編號`, `檔案名稱`, `撥放中`)"
				  "VALUES (null, :Filename, :Playing)");
	Query.bindValue(":Filename", Filename);
	Query.bindValue(":Playing", false);

	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return false;
	}

	return true;
}

QString Sutra_Database::First()
{
	QString		Result = "";
	QSqlQuery	Query;

	Query.prepare("SELECT `檔案名稱` FROM `經文` LIMIT 1");
	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return Result;
	}

	if(Query.next())
	{
		Result = Query.value("檔案名稱").toString();
	}

	return Result;
}
int Sutra_Database::Search(QString Filename)
{
	int			Result = 0;
	QSqlQuery	Query;

	Query.prepare("SELECT `編號` FROM `經文` WHERE `檔案名稱` = :Filename");
	Query.bindValue(":Filename", Filename);
	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return Result;
	}

	if(Query.next())
	{
		Result = Query.value("編號").toInt();
	}

	return Result;
}
QString Sutra_Database::Search(int File_id)
{
	QString		Result = "";
	QSqlQuery	Query;

	Query.prepare("SELECT `檔案名稱` FROM `經文` WHERE `編號` = :File_id");
	Query.bindValue(":File_id", File_id);
	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return Result;
	}

	if(Query.next())
	{
		Result = Query.value("檔案名稱").toString();
	}

	return Result;
}
bool Sutra_Database::Plaing(int File_id)
{
	QSqlQuery	Query;

	Query.prepare("UPDATE `經文` SET `撥放中` = 0");
	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return false;
	}
	Query.prepare("UPDATE `經文` SET `撥放中` = 1 WHERE `編號` = :File_id");
	Query.bindValue(":File_id", File_id);
	if(Query.exec()==false)
	{
		QString ErrorMessage = QString("Unable to exec SQL Command<%1>: %2")
							   .arg(Query.executedQuery())
							   .arg(Query.lastError().text());
		qCritical() << ErrorMessage;
		return false;
	}

	return true;
}
