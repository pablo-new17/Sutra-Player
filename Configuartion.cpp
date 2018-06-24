#include <QFileDialog>
#include <QStringListModel>

#include "Configuartion.h"
#include "ui_Configuartion.h"

Configuartion::Configuartion(QSettings* Config,
							 Sutra_Database* DB,
							 QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Configuartion)
{
	ui->setupUi(this);
	this->Config = Config;
	this->DB = DB;

	Config->beginGroup("DATABASE");
	ui->lineEdit_Database->setText(Config->value("Server").toString());
	Config->endGroup();
	Config->beginGroup("LOCATION");
	ui->lineEdit_TopFolder->setText(Config->value("Top_Folder").toString());
	Config->endGroup();

	int		Index = DB->Search(DB->First());
	QString File = DB->First();
	QStringListModel* Model = new QStringListModel(this);
	ui->listView_PlayList->setModel(Model);

	while(!File.isEmpty())
	{
		Model->insertRows(Model->rowCount(), 1);
		QModelIndex ModelIndex = Model->index(Model->rowCount() - 1, 0);
		Model->setData(ModelIndex, File);
		ui->listView_PlayList->edit(ModelIndex);

		File = DB->Search(++Index);
	}

	this->reload = false;
}

Configuartion::~Configuartion()
{
	delete ui;
}

void Configuartion::on_buttonBox_accepted()
{
	Config->beginGroup("DATABASE");
	Config->setValue("Server", ui->lineEdit_Database->text());
	Config->endGroup();

	Config->beginGroup("LOCATION");
	Config->setValue("Top_Folder", ui->lineEdit_TopFolder->text());
	Config->endGroup();

	if(this->reload)
	{
		Config->beginGroup("SUTRA");
		Config->setValue("Created", 0);
		Config->setValue("Current", "");
		Config->endGroup();
		DB->Clean();
	}

	Config->sync();
}

void Configuartion::on_pushButton_Database_clicked()
{
	QString database = QFileDialog::getSaveFileName(this,
												  "Database location",
												  QDir::currentPath(),
												  "*.db");

	ui->lineEdit_Database->setText(database);
}

void Configuartion::on_pushButton_TopFolder_clicked()
{
	QString folder = QFileDialog::getExistingDirectory(
				this,
				tr("Open Directory"),
				QDir::currentPath(),
				QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->lineEdit_TopFolder->setText(folder);

	this->reload = true;
}

void Configuartion::on_pushButton_Playat_clicked()
{
	QStringListModel* Model = (QStringListModel*)ui->listView_PlayList->model();
	QModelIndex ModelIndex = ui->listView_PlayList->currentIndex();

	QString File = Model->data(ModelIndex).toString();
	int Index = DB->Search(File);
	DB->Plaing(Index);
	Config->beginGroup("SUTRA");
	Config->setValue("Current", File);
	Config->endGroup();
	Config->sync();
}
