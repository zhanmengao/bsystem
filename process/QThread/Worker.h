#ifndef WORKER_H
#define WORKER_H

#include<QObject>
#include<QString>

class Worker:public QObject
{
    Q_OBJECT
public slots:
    void DoWork();
    void WorkEnd();
};


#endif // WORKER_H
