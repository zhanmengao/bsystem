#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QVBoxLayout>
#include<QScrollArea>
#include<QList>
#include"fileitem.h"
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
    void OnClickFile();
    void OnClickCommit();
    void OnLog(const QString& log);
    void OnUsersStdout();
    void OnUsersStdError();
private:
    Ui::MainWindow *ui;
    QVBoxLayout *vLayout;
    QScrollArea *mArea;
    int totalHight = 0;
    QList<FileItem*> mList;
    QProcess *users;
};
#endif // MAINWINDOW_H
