#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMetaType>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QThread>
/*************************************/
class SendParam
{
public:
    uint id;
    double val;
    SendParam()
    {
        id=0;
        val=0.0;
    }
};
    Q_DECLARE_METATYPE(SendParam)
/*************************************/

class WorkThread : public QObject
{
	Q_OBJECT
public:
	WorkThread(QObject* parent = nullptr);
	~WorkThread();
public slots:
	void start1();
	void doWork();
    void stop();
    void recvParam(SendParam sendParam);
signals:
	void workFinished();
	void workStart();
    void stopThread();


private:
        bool m_bTestFlag;
        SendParam m_sendParam;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
signals:
    void stopTest();
    void sendParam(SendParam sendParam);
public slots:
	void ThreadStart();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
	Ui::MainWindow *ui;
    SendParam m_sendParam;

	QThread* m_workerThread;
};
#endif // MAINWINDOW_H
