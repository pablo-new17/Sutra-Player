#ifndef GPIO_SYSFS_H
#define GPIO_SYSFS_H

#include <QObject>
#include <QFile>
#include <QFileSystemWatcher>

typedef enum GPIO_DIRECTION
{
	GPIO_DIRECTION_OUT = 0,
	GPIO_DIRECTION_IN,
}GPIO_DIRECTION;



class GPIO_sysfs : public QObject
{
	Q_OBJECT
public:
	explicit GPIO_sysfs(int GPIO_Pin_Number, GPIO_DIRECTION Direction = GPIO_DIRECTION_OUT, QObject *parent = nullptr);
	~GPIO_sysfs();

	bool Set_Value(bool High);

	bool Get_Value();

	bool init();

signals:
	void Input_Value_Changed();

private:
	int			m_GPIO_Number;
	int			m_Pin;
	GPIO_DIRECTION		m_Direction;
	QString			m_Value;
	QFileSystemWatcher	m_Watcher;

private slots:
	void activated();


};

#endif // GPIO_SYSFS_H
