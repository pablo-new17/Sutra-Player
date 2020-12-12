#ifndef SUTRA_DATABASE_H
#define SUTRA_DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>

#include <QString>

class Sutra_Database
{
private:
	QString		m_Database_Server;
	QSqlDatabase	m_Database;

public:
	explicit Sutra_Database(QString Server);

	bool			Create(void);
	bool			Clean(void);
	bool			Add(QString Filename);
	QString			First();

	int			Search(QString Filename);
	QString			Search(int File_id);
	int			Search_Current();

	bool			Plaing(int File_id);
};

#endif // SUTRA_DATABASE_H
