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
//������
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

//������
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
//������
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

//������
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
boost�ṩ�����������������ֱ����ò�ͬʹ�ó�����

boost::lockfree::queue��֧�ֶ�������ߺͶ���������̵߳��������С�
boost::lockfree::stack��֧�ֶ�������ߺͶ���������̵߳�����ջ��
boost::lockfree::spsc_queue�ǽ�֧�ֵ��������ߺ͵����������̵߳��������У���boost::lockfree::queue���ܸ��á�

Boost�������ݽṹ��APIͨ��������ԭ����ʵ��lock-free���������������������
Boost�ṩ��queue�������ó�ʼ�����������Ԫ��ʱ����������������������Զ��������������������ݽṹ�������Ԫ��ʱ������������������������Զ�������
*/

int boostMain()
{
	boost::lockfree::queue<int> queue;
	return 0;
}