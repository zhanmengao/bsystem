#ifndef SEM_H
#define SEM_H
#include<QSemaphore>
#include<QThread>

static QSemaphore sem;
static int numUsed;

class SemProducer:public QThread
{
public:
protected:
    virtual void run() override
    {
        for(int i = 0;i<10;i++)
        {
            sleep(1);
            ++numUsed;
            qDebug("[SemProducer] numUsed :%d",numUsed);
            sem.release();                                  //放1个
        }
    }
};

class SemConsumer:public QThread
{
public:
protected:
    virtual void run() override
    {
        for(int i = 0;i<10;i++)
        {
            //取一个，没有则阻塞
            sem.acquire();
            qDebug("[SemConsumer] numUsed :%d",numUsed);
        }
    }
};
#endif // SEM_H
