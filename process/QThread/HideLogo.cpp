//
// Created by gaochong on 2020/5/9.
//

#include "HideLogo.h"
#include"MainWindow.h"
void HideLogo::run() {

    /**
     *  超时秒后 ,自动隐藏 LOGO 窗口
     * */
    if(MainWindow::Instance())
    {
        QThread::sleep( MainWindow::Instance()->Config().SleepTime);
        hide_logoWindow->hide();

        QThread::run();
    }
}

HideLogo::~HideLogo() {

}


HideLogo::HideLogo( LogoWindow *logoWindow,QObject *parent) : QThread(parent) {

    hide_logoWindow = logoWindow;

}
