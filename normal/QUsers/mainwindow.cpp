#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<unistd.h>
#include<QProcess>
#include<pwd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mArea = new QScrollArea(this);
    mArea->setGeometry(ui->widget->x()-10,ui->widget->y(),
                       ui->widget->width()+20,ui->widget->height());
    mArea->setWidget(ui->widget);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::OnClickFile);
    connect(ui->commitBtn,&QPushButton::clicked,this,&MainWindow::OnClickCommit);
    qDebug("uid = %d,gid = %d,euid = %d,egid = %d",getuid(),getgid(),geteuid(),getegid());
    qDebug("USER = %s ,USERNAME = %s, LOGNAME = %s",getenv("USER"),getenv("USERNAME"),getenv("LOGNAME"));


    users = new QProcess();
    users->setReadChannel(QProcess::StandardOutput);
    users->setReadChannel(QProcess::StandardError);
    connect(users,
            SIGNAL(readyReadStandardOutput()),
            this,
            SLOT(OnUsersStdout()));

    connect(users,
            SIGNAL(readyReadStandardError()),
            this,
            SLOT(OnUsersStdError()));
    users->start("users");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::OnClickFile()
{
    QString user = ui->comboBox->currentText();
    struct passwd* pw = getpwnam(user.toStdString().c_str());
    if(pw)
    {
        //setuid(pw->pw_uid);
    }

    QString selectedFilter;
    QFileDialog::Options op;
    QStringList fileNameList = QFileDialog::getOpenFileNames(this,tr("选择文件"),"/home/" + user,
                                                             tr("All Files (*)"),&selectedFilter,op);
    for(auto it = fileNameList.begin();it!=fileNameList.end();it++)
    {
        FileItem *item = new FileItem(ui->widget);
        item->SetPath(*it);
        item->move(8,totalHight);
        connect(item,&FileItem::PrintLog,this,&MainWindow::OnLog);
        mList.push_back(item);
        totalHight += item->height();
        unsigned uih = ui->widget->height();
        if(totalHight>uih)
        {
            ui->widget->resize(ui->widget->width(),totalHight + 100);
        }
        qDebug("Add file %s",it->toStdString().c_str());
    }
}
void MainWindow::OnLog(const QString& log)
{
    ui->textBrowser->insertPlainText(log + "\n");
}
void MainWindow::OnClickCommit()
{
    QString user = ui->comboBox->currentText();
    struct passwd* pw = getpwnam(user.toStdString().c_str());
    if(!pw)
    {
        OnLog(QString("get userinfo %1 error").arg(user));
        return;
    }
    for(auto it = mList.begin();it!=mList.end();it++)
    {
        if((*it)->GetState())
        {
            (*it)->SetOwner(pw->pw_uid,pw->pw_gid);
        }
    }
}
void MainWindow::OnUsersStdout()
{
    QString standardOutput = QString::fromLocal8Bit(users->readAllStandardOutput());
    qDebug("%s",standardOutput.toStdString().c_str());
    QStringList list = standardOutput.split("\n");
    for(auto it = list.begin();it!=list.end();it++)
    {
        if(it->size()>0)
        {
           ui->comboBox->addItem(*it);
        }
    }
}
void MainWindow::OnUsersStdError()
{

}
