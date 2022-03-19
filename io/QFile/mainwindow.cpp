#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"TextStream.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include"TextStream.h"
#include<QProcess>
#include<QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));          //给窗口添加菜单
    openAct = new QAction(tr("Open"),this);
    openAct->setShortcut(QKeySequence::Open);               //快捷键
    connect(openAct,SIGNAL(triggered()),this,SLOT(OnClickOpen()));
    fileMenu->addAction(openAct);

    openURL = new QAction(tr("OpenURL"),this);
    connect(openURL,SIGNAL(triggered()),this,SLOT(OnClickOpenURL()));
    fileMenu->addAction(openURL);

    textEdit = new QPlainTextEdit(this);
    setCentralWidget(textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnClickOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    PrintfFileInfo(fileName);
    if(!fileName.isEmpty())
    {
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this,tr("Application"),
                                 tr("Cannot read file %1:\n%s.").arg(fileName).arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        textEdit->setPlainText(in.readAll());
        file.close();
    }
}

void MainWindow::OnClickOpenURL()
{
    QString fileName = QFileDialog::getExistingDirectory(this);
    PrintfFileInfo(fileName);
    if(!fileName.isEmpty())
    {
        fileName.replace("/","\\");//将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
        QProcess::startDetached("explorer "+fileName);//打开上面获取的目录

        QDesktopServices::openUrl(QUrl("file:///C:/Documents and Settings/All Users", QUrl::TolerantMode));
    }
}
