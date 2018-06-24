#ifndef CONFIGUARTION_H
#define CONFIGUARTION_H

#include <QDialog>
#include <QSettings>

#include "Sutra_Database.h"

namespace Ui {
	class Configuartion;
}

class Configuartion : public QDialog
{
	Q_OBJECT

public:
	explicit Configuartion(QSettings* Config,
						   Sutra_Database* DB,
						   QWidget *parent = 0);
	~Configuartion();

private slots:
	void on_buttonBox_accepted();

	void on_pushButton_Database_clicked();

	void on_pushButton_TopFolder_clicked();

	void on_pushButton_Playat_clicked();

private:
	Ui::Configuartion*	ui;
	QSettings*			Config;
	Sutra_Database*		DB;
	bool				reload;
};

#endif // CONFIGUARTION_H
