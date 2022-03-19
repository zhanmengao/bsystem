#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QDebug>
#define CMD_PATH "/home/wby/DOc/Space/System/Setting/Envriment/build-Cmdline-Qt_5_12_9_GCC_64bit-Debug/Cmdline "
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xf.PORT = "3389";
    //xf.RDS = "10.0.0.70";
    xf.RDS = "192.168.20.208";

    xf.appName = "excel";
    //xf.exePath = "C:\\Program Files (x86)\\Microsoft Office\\Office15\\EXCEL.EXE";
    xf.exePath = "C:\\Windows\\system32\\notepad.exe";
    xf.exePath = "C:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe";

    xf.PASSWORD = "******";
    xf.USERNAME = "6763404499867205632";

    xf.openMode = 1;
    xf.App_heigh = 1080;
    xf.App_width = 1920;

    xf.DOMAINNAME = "UAMCLUSTER";

    xf.appParameter = "";

    //xf.SetHomeFilePath("/home/wby/DOc/git.txt");


    CmdlineProcess = new QProcess(this);
    vecloudapp_process = new QProcess(this);
    connect(
                CmdlineProcess,
                SIGNAL(started()),
                this,
                SLOT(cmdstarted()));

    connect(
                CmdlineProcess,
                SIGNAL(readyReadStandardOutput()),
                this,
                SLOT(cmdReadStandardOutput()));

    connect(
                CmdlineProcess,
                SIGNAL(readyReadStandardError()),
                this,
                SLOT(cmdRedFromStdError()));

    connect(
                CmdlineProcess,
                SIGNAL(error(QProcess::ProcessError)),
                this,
                SLOT(cmdProcessError(QProcess::ProcessError)));

    connect(
                CmdlineProcess,
                SIGNAL(finished(int, QProcess::ExitStatus)),
                this,
                SLOT(cmdProcessFinished(int, QProcess::ExitStatus)));


    connect(
                vecloudapp_process,
                SIGNAL(started()),
                this,
                SLOT(vecloudappstarted()));

    connect(
                vecloudapp_process,
                SIGNAL(readyReadStandardOutput()),
                this,
                SLOT(vecloudappReadStandardOutput()));

    connect(
                vecloudapp_process,
                SIGNAL(readyReadStandardError()),
                this,
                SLOT(vecloudappRedFromStdError()));

    connect(
                vecloudapp_process,
                SIGNAL(error(QProcess::ProcessError)),
                this,
                SLOT(vecloudappProcessError(QProcess::ProcessError)));

    connect(
                vecloudapp_process,
                SIGNAL(finished(int, QProcess::ExitStatus)),
                this,
                SLOT(vecloudappProcessFinished(int, QProcess::ExitStatus)));

    vecloudapp_process->setReadChannel(QProcess::StandardOutput);
    vecloudapp_process->setReadChannel(QProcess::StandardError);
    CmdlineProcess->setReadChannel(QProcess::StandardOutput);
    CmdlineProcess->setReadChannel(QProcess::StandardError);



    ui->textBrowser->insertPlainText(V2020Xf().GetXfreerdp(xf) + "\n\n");
    ui->textBrowser->insertPlainText(V2019Xf().GetXfreerdp(xf) + "\n\n");

    connect(ui->Start2020,&QPushButton::clicked,this,&MainWindow::OnClickStart2020);
    connect(ui->Start2019,&QPushButton::clicked,this,&MainWindow::OnClickStart2019);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::vecloudappProcessError(QProcess::ProcessError processError)
{
    qDebug() << "vecloudappProcessError()";

    QString outText = vecloudapp;
    outText += " :: ";

    switch (processError)
    {
    case QProcess::FailedToStart:
        outText += "FailedToStart";
        break;

    case QProcess::Crashed:
        outText += "Crashed";
        break;

    case QProcess::Timedout:
        outText += "Timedout";
        break;

    case QProcess::ReadError:
        outText += "ReadError";
        break;

    case QProcess::WriteError:
        outText += "WriteError";
        break;

    case QProcess::UnknownError:
        outText += "UnknownError";
        break;

    default:
        outText += "default";
        break;
    }
}

void MainWindow::vecloudappRedFromStdError()
{
    qDebug() << "vecloudappRedFromStdError()";

    QString strTemp = QString::fromLocal8Bit(vecloudapp_process->readAllStandardError());
    QStringList tmpList;
    tmpList = strTemp.split("\n");

    foreach(QString str, tmpList)
    {
        if (str.length() == 0) continue;
        if(str.indexOf("recursive lock from xf_UpdateWindowArea") != -1)
        {
            continue;
        }
        qDebug() << "Error:  " << str;
        ui->textBrowser->insertPlainText(str);
        ui->textBrowser->insertPlainText("\n");
    }
}

void MainWindow::vecloudappstarted()
{
    qDebug() << "vecloudappstarted()";

}

void MainWindow::vecloudappReadStandardOutput()
{
    qDebug() << "vecloudappReadStandardOutput()";


    QString strTemp = QString::fromLocal8Bit(vecloudapp_process->readAllStandardOutput());
    QStringList tmpList;
    tmpList = strTemp.split("\n");

    foreach(QString str, tmpList)
    {
        if(str.indexOf("recursive lock from xf_UpdateWindowArea") != -1)
        {
            continue;
        }
        qDebug() << "Output:  " << str;
        ui->textBrowser->insertPlainText(str);
        ui->textBrowser->insertPlainText("\n");
    }
    //logoWindow.hide();
}
void MainWindow::vecloudappProcessFinished(int code, QProcess::ExitStatus exitStatus)
{
    qDebug() << "vecloudappProcessFinished()" << exitStatus;

    if (QProcess::ExitStatus::NormalExit == exitStatus)
    {
    }
    else if (QProcess::ExitStatus::CrashExit == exitStatus)
    {
        //异常退出
    }
}
void MainWindow::cmdProcessError(QProcess::ProcessError processError)
{
    qDebug() << "vecloudappProcessError()";

    QString outText = vecloudapp;
    outText += " :: ";

    switch (processError)
    {
    case QProcess::FailedToStart:
        outText += "FailedToStart";
        break;

    case QProcess::Crashed:
        outText += "Crashed";
        break;

    case QProcess::Timedout:
        outText += "Timedout";
        break;

    case QProcess::ReadError:
        outText += "ReadError";
        break;

    case QProcess::WriteError:
        outText += "WriteError";
        break;

    case QProcess::UnknownError:
        outText += "UnknownError";
        break;

    default:
        outText += "default";
        break;
    }
}

void MainWindow::cmdRedFromStdError()
{
    qDebug() << "vecloudappRedFromStdError()";

    QString strTemp = QString::fromLocal8Bit(CmdlineProcess->readAllStandardError());
    QStringList tmpList;
    tmpList = strTemp.split("\n");

    foreach(QString str, tmpList)
    {
        if (str.length() == 0) continue;

        qDebug() << "Error:  " << str;
        ui->textBrowser->insertPlainText(str);
        ui->textBrowser->insertPlainText("\n");
    }
}

void MainWindow::cmdstarted()
{
    qDebug() << "vecloudappstarted()";

}

void MainWindow::cmdReadStandardOutput()
{
    qDebug() << "vecloudappReadStandardOutput()";


    QString strTemp = QString::fromLocal8Bit(CmdlineProcess->readAllStandardOutput());
    QStringList tmpList;
    tmpList = strTemp.split("\n");

    foreach(QString str, tmpList)
    {
        qDebug() << "Output:  " << str;
        ui->textBrowser->insertPlainText(str);
        ui->textBrowser->insertPlainText("\n");
    }
    //logoWindow.hide();
}
void MainWindow::cmdProcessFinished(int code, QProcess::ExitStatus exitStatus)
{
    qDebug() << "vecloudappProcessFinished()" << exitStatus;

    if (QProcess::ExitStatus::NormalExit == exitStatus)
    {
    }
    else if (QProcess::ExitStatus::CrashExit == exitStatus)
    {
        //异常退出
    }
}
void MainWindow::OnClickStart2019()
{
    if(vecloudapp_process!=NULL)
    {
        vecloudapp_process->kill();
    }
    CmdlineProcess->start(CMD_PATH + V2019Xf().GetXfreerdp(xf));
    vecloudapp_process->start(V2019Xf().GetXfreerdp(xf));
}
void MainWindow::OnClickStart2020()
{
    if(vecloudapp_process!=NULL)
    {
        vecloudapp_process->kill();
    }
    CmdlineProcess->start(CMD_PATH + V2020Xf().GetXfreerdp(xf));
    vecloudapp_process->start(V2020Xf().GetXfreerdp(xf));
}

