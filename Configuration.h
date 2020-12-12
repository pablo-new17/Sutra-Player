#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QSettings>

class Configuration : public QObject
{
	Q_OBJECT
public:
	explicit Configuration(QString Top_Folder, QObject *parent = nullptr);

	QString getDatabase_Server() const;
	void setDatabase_Server(const QString Server);

	int getDatabase_Port() const;
	void setDatabase_Port(const int Port);

	QString getDatabase_User() const;
	void setDatabase_User(const QString User);

	QString getDatabase_Pass() const;
	void setDatabase_Pass(const QString Pass);

	QString getLocation_Media_Folder() const;
	void setLocation_Media_Folder(const QString Media_Folder);

	bool getSutra_Created() const;
	void setSutra_Created(const bool Created);

	QString getSutra_Current() const;
	void setSutra_Current(const QString Current);


	QSettings* getConfig() const;

signals:

private:
	QString			m_Top_Folder;
	QSettings*		m_Config;

private slots:
	QString Find_Config_File();

	void Create_Config();
};

#endif // CONFIGURATION_H
