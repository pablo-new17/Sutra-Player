#include <QCoreApplication>
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QSettings>

#include "File_Search.h"
#include "Sutra_Database.h"
#include "Configuartion.h"

#define	HOME_DIR	"/home/pi"

QString Find_Config_File(void)
{
	QString	Filename = "Configuration.ini";

	QFileInfo* Location = new QFileInfo(Filename);
	if(!Location->exists()) Location = new QFileInfo("/etc/" + QCoreApplication::applicationFilePath() + "/" + Filename);
	if(!Location->exists()) Location = new QFileInfo("/etc/" + Filename);
	if(!Location->exists())	Location = new QFileInfo(QString("%1/%2").arg(HOME_DIR).arg(Filename));
	if(!Location->exists())	Location = new QFileInfo(QDir::homePath() + "/" + Filename);
	if(!Location->exists())
	{
		qWarning() << "The configuration file doest not exist";
//		return "";
	}

	return Location->absoluteFilePath();
}

int main(int argc, char *argv[])
{
//	QCoreApplication a(argc, argv);

	QString			Config_File = Find_Config_File();
	QSettings*		Config = new QSettings(Config_File, QSettings::IniFormat);
	if(!Config->childGroups().contains("SUTRA"))
	{
		QString ErrorMessage = QString("No sutra play settings, so create it at %1")
							   .arg(Config_File);
		qWarning() << ErrorMessage;

		Config->setValue("DATABASE/Server", QString("%1/%2").arg( HOME_DIR).arg("Sutra_List.db"));
		Config->setValue("DATABASE/Port", 3306);
		Config->setValue("DATABASE/User", "username");
		Config->setValue("DATABASE/Pass", "password");

		Config->setValue("LOCATION/Top_Folder", "/media/sutra");

		Config->setValue("SUTRA/Created", 0);
		Config->setValue("SUTRA/Current", "");
		Config->sync();
	}
	File_Search		Files(Config);
	Sutra_Database	DB(Config);

	if(argc==2 && QString(argv[1])=="-c")
	{
		QApplication a(argc, argv);

		Configuartion w(Config, &DB);
		w.show();

		return a.exec();
	}

	Config->beginGroup("SUTRA");
	bool Created = Config->value("Created").toBool();
	QString Current = Config->value("Current").toString();
	int	Play_Number = DB.Search(Current);

	if(!Created)
	{
		DB.Clean();
		DB.Create();
		foreach (QString Filename, Files.getFiles())
		{
			DB.Add(Filename);
			qDebug() << Filename;
		}
		Config->setValue("Created", 1);
	}
	Config->endGroup();

	if(Play_Number==0)
	{
		Current = DB.First();
		Play_Number = DB.Search(Current);
	}


	while(true)
	{
		DB.Plaing(Play_Number);
		Config->beginGroup("SUTRA");
		Config->setValue("Current", Current);
		Config->endGroup();
		Config->sync();

		QString Command = QString("mpv -fs %2 > /dev/null 2>&1").arg(Current);
		qDebug() << Command;
		system(Command.toStdString().c_str());

		if((Current = DB.Search(++Play_Number))=="")
		{
			Play_Number=1;
			Current = DB.Search(Play_Number);
		}
	}

	return 0;
}
