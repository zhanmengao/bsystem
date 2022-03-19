#ifndef PRODUCER_H
#define PRODUCER_H


#include<QThread>
#include<QString>
#include"BlockQueue.hpp"
#include<string.h>

static BlockQueue<QString> msgQueue;
class Producer:public QThread
{
public:
    Producer(int id):id(id)
    {
    }
protected:
    //生产者生产消息
    virtual void run() override
    {
        while(1)
        {
            char buf[1024] = {0};
            sprintf(buf,"%d Producer ID %d",id,msgID++);
            msgQueue.Push(buf);
            sleep(2);
        }
    }
private:
    int msgID = 0;
    int id;
};

//消费者
class Consumer:public QThread
{
public:
    Consumer(int id):id(id)
    {
    }
protected:
    //通过虚函数重写实现自定义线程操作
    virtual void run() override
    {
        while(1)
        {
            QString msg = msgQueue.Pop();
            qDebug("%d Consumer - %s",id,msg.toStdString().c_str());
            sleep(3);
        }
    }
protected:
    int id;
};
//机灵的消费者：只等一小会，等不到就去做自己的事
class ConsumerClever:public Consumer
{
public:
    ConsumerClever(int id):Consumer(id)
    {
    }
protected:
    //通过虚函数重写实现自定义线程操作
    virtual void run() override
    {
        QString msg;
        while(1)
        {
            bool hasMsg = msgQueue.Pop(msg);
            if(hasMsg)
            {
                qDebug("%d Process - %s",id,msg.toStdString().c_str());
                sleep(1);
            }
            else
            {
                qDebug("ConsumerClever %d 没等到消息，去做自己的事了",id);
                sleep(2);
            }
        }
    }
};


#endif // PRODUCER_H
