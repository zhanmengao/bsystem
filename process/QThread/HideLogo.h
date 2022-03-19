//
// Created by gaochong on 2020/5/9.
//

#ifndef CLOUDAPPOPEN_HIDELOGO_H
#define CLOUDAPPOPEN_HIDELOGO_H

#include <QThread>

#include "LogoWindow.h"

class HideLogo :public QThread
{

private:
    LogoWindow *hide_logoWindow;

public:
    HideLogo( LogoWindow *logoWindow, QObject *parent);

    virtual ~HideLogo();


protected:
    void run() override;

};


#endif //CLOUDAPPOPEN_HIDELOGO_H
