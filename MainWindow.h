#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

#include "GPIO_sysfs.h"
#include "Configuration.h"
#include "Sutra_Database.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

signals:
	void Playback();

	void Play_Finished();

private:
	Ui::MainWindow*		ui;
	QStringListModel*	m_Model;
	GPIO_sysfs*		m_GPIO;
	Configuration*		m_Config;
	Sutra_Database*		m_DB;
	int			m_Play_Number;
	int			m_Count_Down = 31;
	bool			m_Pause_Count;

private slots:
	void Show_IP_Address();

	void Show_Current_Media();

	void Init();

	// QObject interface
	void on_pushButton_TopFolder_clicked();

	void on_pushButton_Database_clicked();

	void on_pushButton_Playat_clicked();

	void Play_Next();

	void Play_Start();

	void Add_Media_Files(QString Folder);

protected:
	void timerEvent(QTimerEvent* event) override;
};

#endif // MAINWINDOW_H
