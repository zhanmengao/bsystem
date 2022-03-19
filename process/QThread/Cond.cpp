#include"mainwindow.h"

void MainWindow::OnClickCond()
{
    for(auto it = produlist.begin();it!=produlist.end();it++)
    {
        if((*it)->isRunning())
        {
            (*it)->terminate();
            (*it)->wait();
        }
        else
        {
            (*it)->start();
        }
    }
    for(auto it = consulist.begin();it!=consulist.end();it++)
    {
        if((*it)->isRunning())
        {
            (*it)->terminate();
            (*it)->wait();
        }
        else
        {
            (*it)->start();
        }
    }
}
