#ifndef JZLOCKFREELIST_H
#define JZLOCKFREELIST_H
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include <stack>
#include <iostream>
#include <thread>
#include <list>
#include<memory>
#ifdef _WIN32
#include<Windows.h>
#else
#endif

using namespace std;

/*
无锁队列
其实有锁和无锁就是我们平时所说的乐观锁和悲观锁：
加锁是一种悲观的策略，它总是认为每次访问共享资源的时候，总会发生冲突，所以宁愿牺牲性能（时间）来保证数据安全。

无锁是一种乐观的策略，它假设线程访问共享资源不会发生冲突，所以不需要加锁，因此线程将不断执行，不需要停止。一旦碰到冲突，就重试当前操作直到没有冲突为止。
无锁的策略使用一种叫做比较交换的技术（CAS Compare And Swap）来鉴别线程冲突，一旦检测到冲突产生，就重试当前操作直到没有冲突为止。
CAS是系统原语，CAS操作是一条CPU的原子指令，所以不会有线程安全问题。
CAS 的伪码:
template <class T>
bool CAS(T* addr, T expected, T value)
{
if (*addr == expected)
{
*addr = value;
return true;
}
return false;
}
*/

/*
CAS 将 expected 与一个内存地址进行比较，如果比较成功，就将内存内容替换为 new 。
当前大多数机器都在硬件级实现了这个操作，在 Inter 处理器上这个操作是 CMPXCHG ，
因而 CAS 是一个最基础的原子操作。

GCC4.1+版本中支持CAS的原子操作,API接口如下：
**bool** __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)

type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
*/



#define usleep(ms) Sleep(ms/1000)
#define LOCK 0
#define UNLOCK 1
template <typename T>
class JzLockfreeList
{
private:
	std::list<T> list;

private:
	std::atomic<int> mutex;
	int lock = LOCK;
	int unlock = UNLOCK;
public:
	JzLockfreeList() :mutex(0){};
	~JzLockfreeList() {};

	void Lock()
	{
		while (!mutex.compare_exchange_weak(lock, UNLOCK))
		{
			usleep(100);
		}
	}

	void Unlock()
	{
		mutex.compare_exchange_weak(unlock, LOCK);
	}

	void Push(T data)
	{
		Lock();
		list.push_back(data);
		Unlock();
	}

	T Front()
	{
		Lock();
		T data = list.front();
		Unlock();

		return data;
	}

	void PopFront()
	{
		Lock();
		list.pop_front();
		Unlock();
	}

	bool IsEmpty()
	{
		Lock();
		if (list.empty())
		{
			Unlock();
			return true;
		}
		else
		{
			Unlock();
			return false;
		}
	}

	bool Find(T data)
	{
		typename std::list<T>::iterator it;

		Lock();
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (*it == data)
			{
				Unlock();
				return true;
			}
		}

		Unlock();
		return false;
	}
};


#endif


/*
* @brief：compare & swap(CAS)。如果等于expect则swap，否则就返回--是否交换成功, 注意expect如果不相等，会把当前值写入到expected里面。
* 相比于strong，weak可能会出现[spurious wakeup](<http://en.wikipedia.org/wiki/Spurious_wakeup>).
* @param          若x等于expect,则设置为desired 返回true，
*                 否则最新值写入expect，返回false

class atomic
{
bool compare_exchange_strong(T& expect,T desired)			//用来比较的值,  用来设置的值
bool compare_exchange_weak(T& expect, T desired)
}
*/