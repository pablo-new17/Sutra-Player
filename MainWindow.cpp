#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Network_Utility.h"
#include "File_Search.h"

#define TOP_FOLDER	"/home/pi"

#ifdef	FOR_RASPBERRYPI
#define GPIO_ENABLED
#endif

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->m_Play_Number = -1;

#ifdef	GPIO_ENABLED
	this->m_GPIO = new GPIO_sysfs(12, GPIO_DIRECTION_IN, this);
	connect(this->m_GPIO, &GPIO_sysfs::Input_Value_Changed, this, &MainWindow::Play_Next);
#endif

	this->m_Config = new Configuration(TOP_FOLDER, this);

	this->m_DB = new Sutra_Database(this->m_Config->getDatabase_Server());

	this->Show_IP_Address();

	this->m_Model = new QStringListModel(this);
	ui->listView_PlayList->setModel(this->m_Model);

	ui->lineEdit_Database->setText(this->m_Config->getDatabase_Server());
	ui->lineEdit_TopFolder->setText(this->m_Config->getLocation_Media_Folder());

	startTimer(1000);

	connect(this, &MainWindow::Playback, this, &MainWindow::Play_Start);

	this->m_Player = new MediaPlayer(this);

	connect(this->m_Player, &MediaPlayer::Finished, this, &MainWindow::Play_Next);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::Show_IP_Address()
{
	Network_Utility Network;
	QString IP_Address;
	foreach(Network_Device Dev, Network.List_Network_Device())
	{
		if(!IP_Address.isEmpty())
			IP_Address.append("; ");
		IP_Address.append(Dev.IP_Address);
	}

	ui->label_IP_Address->setText(IP_Address);
}

void MainWindow::Show_Current_Media()
{
	QString File = this->m_DB->Search(this->m_Play_Number);

	QModelIndexList indexes = this->m_Model->match(
	  this->m_Model->index(0,0),  // first row, first column
	  Qt::DisplayRole,  // search the text as displayed
	  File,  // there is a QVariant(QString) conversion constructor
	  1, // first hit only
	  Qt::MatchExactly // or Qt::MatchFixedString
	);

	if(indexes.size())
		ui->listView_PlayList->setCurrentIndex(indexes.at(0));
}

void MainWindow::Init()
{
	if(this->m_Config->getSutra_Created())
	{
		this->m_Model = new QStringListModel(this);

		this->m_Play_Number = this->m_DB->Search_Current();

		QString File = this->m_DB->First();
		int Index = this->m_DB->Search(File);

		if(this->m_Play_Number == -1)
		{
			this->m_Play_Number = Index;
		}

		while(!File.isEmpty())
		{
			this->m_Model->insertRows(this->m_Model->rowCount(), 1);
			QModelIndex ModelIndex = this->m_Model->index(this->m_Model->rowCount() - 1, 0);
			this->m_Model->setData(ModelIndex, File);

			File = this->m_DB->Search(++Index);
		}

		ui->listView_PlayList->setModel(this->m_Model);
		ui->pushButton_Playat->setEnabled(this->m_Model->rowCount() > 0);
		Show_Current_Media();
	}
	else
	{
		Add_Media_Files(this->m_Config->getLocation_Media_Folder());
	}
}


void MainWindow::on_pushButton_TopFolder_clicked()
{
	this->m_Pause_Count = true;

	QString folder = QFileDialog::getExistingDirectory(
				 this,
				 tr("Open Directory"),
				 QDir::currentPath(),
				 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(!folder.isEmpty())
	{
		QDir Dir_check(folder);
		if(Dir_check.exists())
		{
			ui->lineEdit_TopFolder->setText(folder);

			this->m_Config->setLocation_Media_Folder(folder);
			this->m_Config->setSutra_Created(false);
			this->m_Config->setSutra_Current("");

			Add_Media_Files(folder);
		}
		else
		{
			QMessageBox::warning(this, "目錄不存在", "所指定的目錄錯誤，或不存在，請確認後重試");
		}
	}

	this->m_Pause_Count = false;
	this->m_Count_Down = 30;
}

void MainWindow::on_pushButton_Database_clicked()
{
	this->m_Pause_Count = true;

	QString database = QFileDialog::getSaveFileName(
				   this,
				   "Database location",
				   QDir::currentPath(),
				   "*.db");

	if(!database.isEmpty())
	{
		if(QFileInfo::exists(database))
		{
			ui->lineEdit_Database->setText(database);

			this->m_Config->setDatabase_Server(database);
		}
		else
		{
			QMessageBox::warning(this, "資料庫不存在", "所指定的資料庫錯誤，或不存在，請確認後重試");
		}
	}

	this->m_Pause_Count = false;
	this->m_Count_Down = 30;
}

void MainWindow::on_pushButton_Playat_clicked()
{
	QModelIndex index = ui->listView_PlayList->currentIndex();
	QString itemText = index.data(Qt::DisplayRole).toString();

	this->m_Play_Number = this->m_DB->Search(itemText);

	qDebug() << this->m_Play_Number << itemText ;

	if(this->m_Count_Down >= 0)
		this->m_Count_Down = 0;
	else
		emit Playback();
}

void MainWindow::Play_Next()
{
	this->m_Play_Number++;

	this->Play_Start();
}

void MainWindow::Play_Start()
{
	QString Filename = this->m_DB->Search(this->m_Play_Number);

	qDebug() << "Play_Next" << this->m_Play_Number << Filename;

	if(Filename.isEmpty())
	{
		this->m_Play_Number = 1;

		Filename = this->m_DB->Search(this->m_Play_Number);
	}

	this->m_DB->Plaing(this->m_Play_Number);

	this->Show_Current_Media();

	this->m_Player->Play(Filename);
}

void MainWindow::Add_Media_Files(QString Folder)
{
	File_Search	Files(Folder);

	this->m_Model = new QStringListModel(this);

	this->m_DB->Clean();
	this->m_DB->Create();

	foreach (QString Filename, Files.getFiles())
	{
		this->m_DB->Add(Filename);

		this->m_Model->insertRows(this->m_Model->rowCount(), 1);
		QModelIndex ModelIndex = this->m_Model->index(this->m_Model->rowCount() - 1, 0);
		this->m_Model->setData(ModelIndex, Filename);

		qDebug() << Filename;
	}

	this->m_Config->setSutra_Created(true);
	ui->listView_PlayList->setModel(this->m_Model);
	ui->pushButton_Playat->setEnabled(this->m_Model->rowCount() > 0);


	QString File = this->m_DB->First();
	this->m_Play_Number = this->m_DB->Search(File);
	Show_Current_Media();

	emit Playback();
}

void MainWindow::timerEvent(QTimerEvent* event)
{
	if(!this->m_Pause_Count)
	{
		this->m_Count_Down--;
		qDebug() << "Count_Down" << this->m_Count_Down;

		//開始起始化
		if(this->m_Count_Down == 30)
		{
			Init();
		}

		if(this->m_Count_Down < 0)
		{
			ui->statusbar->showMessage("");
			killTimer(event->timerId());
			emit Playback();
		}
		else
			ui->statusbar->showMessage(QString("將於 %1 秒後自動撥放").arg(this->m_Count_Down));
	}
}

void MainWindow::on_pushButton_clicked()
{
	this->m_Player->Cancel();
}
