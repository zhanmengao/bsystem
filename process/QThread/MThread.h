#ifndef MTHREAD_H
#define MTHREAD_H
#include<QThread>
#include<QMutex>
class AdvancedThread:public QThread
{
public:
    AdvancedThread(int id);
protected:
    //通过虚函数重写实现自定义线程操作
    virtual void run() override;
public:
    void stop();
private:
    bool threadStop;
    int id;
    static int value;
    static QMutex mMutex;
};

#endif // MTHREAD_H
