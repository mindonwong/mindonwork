#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <synchapi.h>

#include <QDebug>
#include <QThread>

class KThread : public QThread
{
public:
	KThread(QObject *parent = NULL) : QThread(parent)
	{
	}
	~KThread()
	{
		qDebug()<<"QThread Destory"<<endl;
	}
};

WorkThread::WorkThread(QObject* parent) : QObject (parent)
{
}
WorkThread::~WorkThread()
{
	qDebug()<<"WorkThread Destory"<<endl;
}
void WorkThread::start1()
{
	emit workStart();
    m_bTestFlag=true;
	doWork();
}
void WorkThread::doWork()
{
    //for (int i = 0; i < 1000; i++)
    while(m_bTestFlag)
	{
        qDebug()<<m_sendParam.id<<endl;
        Sleep(1000);
	}
	emit workFinished();
}
void WorkThread::stop()
{
    m_bTestFlag=false;
}
void WorkThread::recvParam(SendParam sendParam)
{
        m_sendParam=sendParam;
}
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    qRegisterMetaType<SendParam>("SendParam");

	m_workerThread = new KThread();
	WorkThread* worker = new WorkThread();
	worker->moveToThread(m_workerThread);

	//开始线程
	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ThreadStart);
	connect(m_workerThread, &KThread::started, worker, &WorkThread::start1);

	//
	connect(worker, &WorkThread::workFinished, m_workerThread, [this]()
	{
		m_workerThread->quit();
		m_workerThread->wait();
	});

	connect(m_workerThread, &QThread::finished, worker, &WorkThread::deleteLater);
	connect(worker, &WorkThread::destroyed, m_workerThread, &QThread::deleteLater);
    connect(this,&MainWindow::stopTest,worker,&WorkThread::stop);
    connect(this,&MainWindow::sendParam,worker,&WorkThread::recvParam);

	//销毁线程
//	connect(worker, &WorkThread::workFinished, worker, &WorkThread::deleteLater);
//	connect(worker, &WorkThread::destroyed, m_workerThread, &KThread::quit);
//	connect(m_workerThread, &KThread::finished, m_workerThread, &KThread::deleteLater);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::ThreadStart()
{
	m_workerThread->start();
}


//测试结束
void MainWindow::on_pushButton_2_clicked()
{
    emit stopTest();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_sendParam.id++;
    emit sendParam(m_sendParam);
}
