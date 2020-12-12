#include <QDebug>
#include <QDir>

#include "File_Search.h"

QStringList File_Search::getFiles() const
{
	return m_Files;
}

void File_Search::Search(QString Folder)
{
	QStringList File_Filter;
	QDir Top(Folder);

	Top.setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	Top.setSorting(QDir::Name);
	File_Filter << "*.mp4" << "*.wmv" << "*.avi" << "*.mkv";
	Top.setNameFilters(File_Filter);

	foreach (QFileInfo FileInfo, Top.entryInfoList())
	{
		if(FileInfo.isDir())
		{
			this->Search(FileInfo.absoluteFilePath());
		}
		else
		{
			this->m_Files.append(FileInfo.absoluteFilePath());
		}
	}
}

File_Search::File_Search(QString Location)
{
	this->m_Files.clear();
	Search(Location);
}

