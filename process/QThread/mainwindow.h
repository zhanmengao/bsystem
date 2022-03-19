#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QThread>
#include<QPushButton>
#include"MThread.h"
#include<QHBoxLayout>
#include"Cond.h"
#include"Sem.h"
#include<QList>
#include<QFuture>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void OnClickAllStart();
    void OnClickStart1();
    void OnClickStart2();
    void OnClickState();
    void OnClickCond();
    void OnClickSem();
    void OnClickConcurrent();

    void OnThreadStart();
    void OnThreadFinish();
public:
    void PutWorkerInThread();
private:
    QThread *workerThread;
    AdvancedThread* advThread = NULL;
    AdvancedThread* advThread2 = NULL;
    QHBoxLayout *hboxLayout;

    QList<Producer*> produlist;
    QList<Consumer*> consulist;

    SemProducer *semProducer;
    SemConsumer *semConsumer;

    QFuture<void> f1;
    QFuture<void> f2;
};
#endif // MAINWINDOW_H
