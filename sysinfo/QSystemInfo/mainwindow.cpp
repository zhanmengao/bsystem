#include "mainwindow.h"
#include"NetInfo.h"
#include"Desktop.h"
#include"SystemInfo.h"
#include"widget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug("%s",qPrintable(NetInfo::GetHostIpAddress()));
    qDebug("%s",qPrintable(NetInfo::GetHostMacAddress()));
    qDebug("%s",qPrintable(SysInfo::GetUsername()));
    qDebug("%s",qPrintable(SysInfo::GetHostName()));
    //QList<QScreen*> list = DesktopInfo::GetScreenList();
    //qDebug("%d",list.count());
    qDebug("%d",DesktopInfo::GetScreenCount());
    qDebug("%d",DesktopInfo::GetWidth());
    qDebug("%d",DesktopInfo::GetHeight());
    setCentralWidget(new Widget(this));
}

MainWindow::~MainWindow()
{
}

