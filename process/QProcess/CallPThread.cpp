//
// Created by gaochong on 2020/5/7.
//

#include <QMessageBox>

#include "CallPThread.h"
#include "VEREMOTEAPP.h"


void CallPThread::run()
{
    /**
     * 在多线程下信号触发需要特殊处理
     **/
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    cloudAppOpenProcess = new QProcess();
    QString processCMD = VEREMOTEAPP;

    connect(cloudAppOpenProcess,
            SIGNAL(started()),
            this,
            SLOT(cloudAppOpenProcessStarted()));

    connect(cloudAppOpenProcess,
            SIGNAL(readyReadStandardOutput()),
            this,
            SLOT(cloudAppOpenProcessReadStandardOutput()));

    connect(cloudAppOpenProcess,
            SIGNAL(readyReadStandardError()),
            this,
            SLOT(cloudAppOpenProcessRedFromStdError()));

    connect(cloudAppOpenProcess,
            SIGNAL(error(QProcess::ProcessError)),
            this,
            SLOT(cloudAppOpenProcessError(QProcess::ProcessError)));

    connect(cloudAppOpenProcess,
            SIGNAL(finished(int, QProcess::ExitStatus)),
            this,
            SLOT(cloudAppOpenProcessFinished(int, QProcess::ExitStatus)));

    cloudAppOpenProcess->setReadChannel(QProcess::StandardError);
    cloudAppOpenProcess->setReadChannel(QProcess::StandardOutput);

    qDebug() << "AppMenuBox ==> CallPThread::run()";
    cloudAppOpenProcess->start(processCMD ,cmmandStr );

    QThread::run();

}

CallPThread::CallPThread(QStringList argString,  QObject *parent) : QThread(parent)
{

    this->cmmandStr = argString;
}

CallPThread::~CallPThread()
{

}

bool CallPThread::event(QEvent *event)
{

    return QThread::event(event);
}

//////////////////////////////////////////////////////////////////////////////////////////


void CallPThread::cloudAppOpenProcessStarted()
{

    qDebug() << "cloudAppOpenProcessStarted()";
}

void CallPThread::cloudAppOpenProcessReadStandardOutput()
{
    //qDebug() << "cloudAppOpenProcessReadStandardOutput()";
    QString standardError = QString::fromLocal8Bit(cloudAppOpenProcess->readAllStandardError());
    QStringList tmpList;
    tmpList.clear();
    tmpList = standardError.split("\n");

    foreach(QString lstr, tmpList)
    {
            if (lstr.length() == 0)
                continue;

            qDebug() << "cloudAppOpenProcessRedFromMsg: " << lstr;
    }
}

void CallPThread::cloudAppOpenProcessRedFromStdError(){

    //qDebug() << "cloudAppOpenProcessReadStandardOutput():";

    QString standardError = QString::fromLocal8Bit(cloudAppOpenProcess->readAllStandardError());
    QStringList tmpList;
    tmpList.clear();
    tmpList = standardError.split("\n");

    foreach(QString lstr, tmpList)
    {
        if (lstr.length() == 0)
            continue;
        qDebug() << "cloudAppOpenProcessRedFromMsg: " << lstr;
    }

}

void CallPThread::cloudAppOpenProcessError(QProcess::ProcessError processError)
{
    qDebug() << "cloudAppOpenProcessError():" << processError;

    if (!QFile::exists(VEREMOTEAPP))
    {

        qDebug() << "VECloudApp 模块缺失，无法启动云应用" << VEREMOTEAPP;
        QString filename =  VEREMOTEAPP;
        QMessageBox::warning(NULL,
                             "云应用",
                             "VECloudApp 模块缺失，无法启动云应用, 缺失的文件：" + filename,
                             QMessageBox::Yes);
        //return;
    }

}

void CallPThread::cloudAppOpenProcessFinished(int code, QProcess::ExitStatus exitStatus)
{
    qDebug() << "cloudAppOpenProcessFinished():" << exitStatus << code;
    this->exit(0);
}
