#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include<QQueue>
#include<QWaitCondition>
#include<QMutex>

template<class T>
class BlockQueue
{
public:
    void Push(const T& obj)
    {
        mMutex.lock();
        while(mQueue.size()>=500)
        {
            mNonFullCond.wait(&mMutex);                 //等待pop
        }
        mQueue.push_back(obj);
        mMutex.unlock();
        mNonEmptyCond.wakeOne();                        //唤醒消费者
    }
    T Pop()
    {
        mMutex.lock();
        while(mQueue.empty())
        {
            mNonEmptyCond.wait(&mMutex,ULONG_MAX);                 //阻塞等待Push，死等，可以使用参数2设置时间
        }
        T obj = mQueue.front();
        mQueue.pop_front();
        //唤醒生产者
        mMutex.unlock();
        mNonFullCond.wakeOne();
        return obj;
    }
    bool Pop(T& obj)
    {
        mMutex.lock();
        if(mQueue.empty())
        {
            mNonEmptyCond.wait(&mMutex,1);                 //阻塞等待Push,只等待1毫秒
            //如果醒了以后发现还是空，就不处理了
            if(mQueue.empty())
            {
                mMutex.unlock();
                return false;
            }
            obj = mQueue.front();
            mQueue.pop_front();
            //唤醒生产者
            mMutex.unlock();
            mNonFullCond.wakeOne();
            return true;
        }
        else
        {
            obj = mQueue.front();
            mQueue.pop_front();
            //唤醒生产者
            mMutex.unlock();
            mNonFullCond.wakeOne();
            return true;
        }
    }
private:
    QQueue<T> mQueue;
    QMutex mMutex;
    QWaitCondition mNonFullCond;
    QWaitCondition mNonEmptyCond;
};
#endif // BLOCKQUEUE_H
