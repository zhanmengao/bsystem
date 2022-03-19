#include "mainwindow.h"

#include"Worker.h"
#include<QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800,600);
    PutWorkerInThread();
    QWidget* mainWidget=new QWidget;
    hboxLayout = new QHBoxLayout();
    advThread = new AdvancedThread(0);
    advThread2 = new AdvancedThread(1);
    advThread2->setPriority(QThread::HighPriority);                 //设置比最高优先级低的线程优先级
    advThread2->start();
    //注册信号
    connect(advThread,&QThread::started,this,&MainWindow::OnThreadStart);
    connect(advThread,&QThread::finished,this,&MainWindow::OnThreadFinish);
    connect(advThread2,&QThread::started,this,&MainWindow::OnThreadStart);
    connect(advThread2,&QThread::finished,this,&MainWindow::OnThreadFinish);

    for(int i = 0;i<15;i++)
    {
        produlist.push_back(new Producer(i));
    }
    for(int i = 0;i<20;i++)
    {
        consulist.push_back(new Consumer(i));
    }
    consulist.push_back(new ConsumerClever(99));

    semConsumer = new SemConsumer();
    semProducer = new SemProducer();

    QPushButton *startBtn1 = new QPushButton("线程1");
    startBtn1->setGeometry(50,50,100,100);
    connect(startBtn1,&QPushButton::clicked,this,&MainWindow::OnClickStart1);

    QPushButton *startBtn2 = new QPushButton("线程2");
    startBtn2->setGeometry(50,50,100,100);
    connect(startBtn2,&QPushButton::clicked,this,&MainWindow::OnClickStart2);


    QPushButton *stateBtn = new QPushButton("状态");
    stateBtn->setGeometry(120,50,50,50);
    connect(stateBtn,&QPushButton::clicked,this,&MainWindow::OnClickState);

    QPushButton *condBtn = new QPushButton("条件变量");
    condBtn->setGeometry(50,50,100,100);
    connect(condBtn,&QPushButton::clicked,this,&MainWindow::OnClickCond);

    QPushButton *semBtn = new QPushButton("信号量");
    semBtn->setGeometry(50,50,100,100);
    connect(semBtn,&QPushButton::clicked,this,&MainWindow::OnClickSem);

    QPushButton *conBtn = new QPushButton("Concurrent");
    conBtn->setGeometry(50,50,100,100);
    connect(conBtn,&QPushButton::clicked,this,&MainWindow::OnClickConcurrent);

    mainWidget->setLayout(hboxLayout);
    hboxLayout->addWidget(startBtn1);
    hboxLayout->addWidget(startBtn2);
    hboxLayout->addWidget(stateBtn);
    hboxLayout->addWidget(condBtn);
    hboxLayout->addWidget(semBtn);
    hboxLayout->addWidget(conBtn);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    workerThread->exit();
    workerThread->quit();
    workerThread->wait();

    advThread->stop();
    advThread->wait();

    advThread2->terminate();
    advThread2->wait();
}

void hello(QString name)
{
    qDebug("Hello %s from %id",name.toStdString().c_str(),QThread::currentThreadId());
    for(int i = 0;i<2;i++)
    {
        QThread::sleep(1);
        qDebug("[%s] i = %d",name.toStdString().c_str(),i);
    }
}
void MainWindow::OnClickConcurrent()
{
    //QFuture用于显示其他异步线程执行结果
    f1 = QtConcurrent::run(hello,QString("Alice"));                 //在线程中运行生成的函数
    f1.waitForFinished();                                           //等待线程结束


    f2 = QtConcurrent::run(hello,QString("Bob"));
    f2.waitForFinished();
}
