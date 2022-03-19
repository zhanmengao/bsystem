#pragma once
#include <iostream>
#include <thread>
#include<mutex>
#include<atomic>
using namespace std;

/*
CAS即Compare and Swap，是所有CPU指令都支持CAS的原子操作（X86中CMPXCHG汇编指令），用于实现实现各种无锁（lock free）数据结构。
CAS操作的C语言实现如下：

bool compare_and_swap ( int *memory_location, int expected_value, int new_value)
{
	if (*memory_location == expected_value)
	{
		*memory_location = new_value;
		return true;
	}
	return false;
}
*/
void threadUnsafe(int &num)
{
	for (int i = 0; i < 1000000; i++)
	{
		num++;
	}
}

std::mutex mux;
void threadLock(int &num)
{
	printf("begin time %ld \n", time(NULL));
	for (int i = 0; i < 10000000; i++)
	{
		std::lock_guard<std::mutex> lock(mux);
		num++;
	}
	printf("end time %ld \n", time(NULL));
}

void threadAtomicValue(std::atomic<int> &num)
{
	printf("begin time %ld \n", time(NULL));
	for (int i = 0; i < 100000; i++)
	{
		num++;
	}
	printf("end time %ld \n", time(NULL));
}

int mainAtomicValue()
{
	{
		int count = 0;
		thread thread1(threadLock, ref(count));
		thread thread2(threadLock, ref(count));
		thread1.join();
		thread2.join();
		cout << "最后的结果，count：" << count << endl;
	}

	{
		atomic_int count = 0;
		thread thread1(threadAtomicValue, ref(count));
		thread thread2(threadAtomicValue, ref(count));
		thread1.join();
		thread2.join();
		cout << "最后的结果，count：" << count << endl;
	}
	return 1;
}