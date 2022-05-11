#include"freeQueue.h"
#include<time.h>
#include<thread>
#include<boost\lockfree\queue.hpp>
#include"locklist.h"
#include<atomic>
#ifdef _WIN32
#include<Windows.h>
#define sleep(ms) Sleep(ms/1000)
#else
#include <unistd.h>
#endif
//生产者
void Producer(FreeQueue* queue)
{
	while (true)
	{
		num++;
		cout << "num push:" << num << endl;
		LF.Push(num);
		sleep(2);
	}
}

//消费者
void Customer(FreeQueue* queue)
{
	while (true)
	{
		if (!LF.IsEmpty())
		{
			cout << "num get " << LF.Front() << endl;
			LF.PopFront();
		}
		sleep(1);
	}
}

int main()
{
	FreeQueue que;
	for (int i = 0; i < 1000; i++)
	{
		auto tm = time(NULL);
		que.EnQueueV1(new int(tm));
	}
	return 0;
}

JzLockfreeList<int> LF;

thread_local int num = 1;
//生产者
void Producer()
{
	while (true)
	{
		num++;
		cout << "num push:" << num << endl;
		LF.Push(num);
		sleep(2);
	}
}

//消费者
void Customer()
{
	while (true)
	{
		if (!LF.IsEmpty())
		{
			cout << "num get " << LF.Front() << endl;
			LF.PopFront();
		}
		sleep(1);
	}
}
int main2()
{
	thread t1(Producer);
	thread t2(Customer);
	thread t3(Customer);
	if (t1.joinable())
	{
		t1.join();
	}
	if (t2.joinable())
	{
		t2.join();
	}
	if (t3.joinable())
	{
		t3.join();
	}
	return 0;
}


/*
boost提供了三种无锁方案，分别适用不同使用场景。

boost::lockfree::queue是支持多个生产者和多个消费者线程的无锁队列。
boost::lockfree::stack是支持多个生产者和多个消费者线程的无锁栈。
boost::lockfree::spsc_queue是仅支持单个生产者和单个消费者线程的无锁队列，比boost::lockfree::queue性能更好。

Boost无锁数据结构的API通过轻量级原子锁实现lock-free，不是真正意义的无锁。
Boost提供的queue可以设置初始容量，添加新元素时如果容量不够，则总容量自动增长；但对于无锁数据结构，添加新元素时如果容量不够，总容量不会自动增长。
*/

int boostMain()
{
	boost::lockfree::queue<int> queue;
	return 0;
}