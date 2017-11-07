#ifndef FILE_SEARCH_H
#define FILE_SEARCH_H

#include <QStringList>
#include <QSettings>

class File_Search
{
private:
	QStringList	m_Files;

	void Search(QString Folder);


public:
	File_Search(QString Location);
	File_Search(QSettings* Config);

	QStringList getFiles() const;
};

#endif // FILE_SEARCH_H
