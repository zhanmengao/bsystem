#include"mainwindow.h"

void MainWindow::OnClickSem()
{
    if(semProducer->isRunning())
    {
        semProducer->terminate();
    }
    else
    {
        semProducer->start();
    }
    if(semConsumer->isRunning())
    {
        semConsumer->terminate();
    }
    else
    {
        semConsumer->start();
    }
}
