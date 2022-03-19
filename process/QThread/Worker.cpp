#include"Worker.h"
#include"mainwindow.h"
void Worker::DoWork()
{
    qDebug("DoWork..");
}
void Worker::WorkEnd()
{
    qDebug("Worker End..");
}

void MainWindow::OnThreadStart()
{
    qDebug("%d advThread Is Start",QThread::currentThread()->currentThreadId());
}
void MainWindow::OnThreadFinish()
{
    qDebug("%d advThread Is Finish",QThread::currentThread()->currentThreadId());
}

void MainWindow::PutWorkerInThread()
{
    Worker *worker = new Worker();
    workerThread = new QThread(this);
    //注册信号
    connect(workerThread,&QThread::started,worker,&Worker::DoWork);
    connect(workerThread,&QThread::finished,worker,&Worker::WorkEnd);

    worker->moveToThread(workerThread);
    workerThread->start();
}
