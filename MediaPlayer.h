#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QThread>
#include <QProcess>

class MediaPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MediaPlayer(QObject *parent = nullptr);

public slots:
	void Play(const QString Media_File);
	void Cancel();

signals:
	void Playing();
	void Finished();


private:
	QThread*	m_Thread;
	QProcess*	m_Player;

private slots:
//	void Thread_Started();
//	void Thread_Finished();

};

#endif // MEDIAPLAYER_H
