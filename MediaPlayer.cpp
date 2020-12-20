#include <QDebug>

#include "MediaPlayer.h"

#ifdef	FOR_RASPBERRYPI
#define MEDIA_PLAYER	"mpv"
#else
#define MEDIA_PLAYER	"C:/Program Files (x86)/VideoLAN/VLC/vlc.exe"
#endif

MediaPlayer::MediaPlayer(QObject *parent) : QObject(parent)
{
	this->m_Player = new QProcess(this);

	connect(this->m_Player, &QProcess::started, this, &MediaPlayer::Playing);
	connect(this->m_Player, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &MediaPlayer::Finished);
}

void MediaPlayer::Play(const QString Media_File)
{
	this->m_Player->setProgram(MEDIA_PLAYER);

#ifdef	FOR_RASPBERRYPI
	this->m_Player->setEnvironment(QStringList() << "DISPLAY=:0.0");

	this->m_Player->setArguments(QStringList() << "-fs" << Media_File);
#else
	this->m_Player->setArguments(QStringList() << QString("file:///%1").arg(Media_File));
#endif
	qDebug() << "Run" << this->m_Player->program() << this->m_Player->arguments();

	this->m_Player->start();
}

void MediaPlayer::Cancel()
{
	this->m_Player->kill();
}
