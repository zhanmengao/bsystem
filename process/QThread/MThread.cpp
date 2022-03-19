#include"MThread.h"
#include"mainwindow.h"
AdvancedThread::AdvancedThread(int id):id(id)
{
}
void AdvancedThread::run()
{
    threadStop = false;
    while(!threadStop)
    {
        //lock
        {
            QMutexLocker locker(&mMutex);
            qDebug("[%d] AdvancedThread %d",id,value);
            value++;
        }
        msleep(200);
    }
    //exec();
}
void AdvancedThread::stop()
{
    threadStop = true;
    qDebug("[%d] Thread stop",id);
}
void MainWindow::OnClickAllStart()
{
    if(!advThread->isRunning())
    {
        advThread->start();
    }
    else
    {
        advThread->stop();
        advThread->wait();
    }
    if(!advThread2->isRunning())
    {
        advThread2->start();
    }
    else
    {
        advThread2->stop();
        advThread2->wait();
    }
}
void MainWindow::OnClickStart1()
{
    if(!advThread->isRunning())
    {
        advThread->start();
    }
    else
    {
        advThread->stop();
        advThread->wait();
    }
}
void MainWindow::OnClickStart2()
{
    if(!advThread2->isRunning())
    {
        advThread2->start();
    }
    else
    {
        advThread2->stop();
        advThread2->wait();
    }
}

void MainWindow::OnClickState()
{
    if(workerThread == NULL)
    {
        qDebug("workerThread Is NULL");
    }
    else if(workerThread->isRunning())
    {
        qDebug("workerThread Is Running");
    }
    else if(workerThread->isFinished())
    {
        qDebug("workerThread Is isFinished");
    }
    else
    {
        qDebug("workerThread Is unknow");
    }

    if(advThread == NULL)
    {
        qDebug("advThread Is NULL");
    }
    else if(advThread->isRunning())
    {
        qDebug("advThread Is Running");
    }
    else if(advThread->isFinished())
    {
        qDebug("advThread Is isFinished");
    }
    else
    {
        qDebug("advThread Is unknow");
    }
}
int AdvancedThread::value = 0;
QMutex AdvancedThread::mMutex;
