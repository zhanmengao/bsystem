#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"../../../../Network/RDP/UAMLinux/GetCloudAppOpenFile2020/Strategy.h"
#include<QProcess>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void OnClickStart2019();
    void OnClickStart2020();
    void cmdReadStandardOutput();
    void cmdRedFromStdError();
    void cmdProcessError(QProcess::ProcessError processError);
    void cmdProcessFinished(int code, QProcess::ExitStatus exitStatus);
    void cmdstarted();

    void vecloudappReadStandardOutput();
    void vecloudappRedFromStdError();
    void vecloudappProcessError(QProcess::ProcessError processError);
    void vecloudappProcessFinished(int code, QProcess::ExitStatus exitStatus);
    void vecloudappstarted();
private:
    Ui::MainWindow *ui;
    xFreerdp xf;
    QProcess *vecloudapp_process;
    QProcess* CmdlineProcess;
};
#endif // MAINWINDOW_H
