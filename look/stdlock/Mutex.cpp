#include "Define.h"

void MutexTest::Push(int n, char c)
{
	m_mutex.lock();
	for (int i = 0; i < n; ++i)
	{
		m_charStack.push(c);
		std::cout << c;
	}
	std::cout << std::endl;
	m_mutex.unlock();
}


void MutexMain()
{
	MutexTest test;
	std::thread mutexTestThread1(&MutexTest::Push, &test, 10, 'a');
	std::thread mutexTestThread2(&MutexTest::Push, &test, 10, 'b');

	mutexTestThread1.join();
	mutexTestThread2.join();
}


//std::unique_lock以独占所有权的方式来管理mutex对象的上锁和解锁操作。
std::mutex foo, bar;
void task_a()
{
	std::lock(foo, bar);
	std::unique_lock<std::mutex> lck1(foo, std::adopt_lock);
	std::unique_lock<std::mutex> lck2(bar, std::adopt_lock);
	std::cout << "task a\n";
}

void task_b()
{
	std::unique_lock<std::mutex> lck1, lck2;
	lck1 = std::unique_lock<std::mutex>(bar, std::defer_lock);
	lck2 = std::unique_lock<std::mutex>(foo, std::defer_lock);
	std::lock(lck1, lck2);
	std::cout << "task b\n";
}


int UniqueMutexMain()
{
	std::thread th1(task_a);
	std::thread th2(task_b);

	th1.join();
	th2.join();

	return 0;
}

