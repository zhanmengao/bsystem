#pragma once
#include <iostream>
#include <thread>
#include<mutex>
#include<atomic>
using namespace std;

/*
CAS��Compare and Swap��������CPUָ�֧��CAS��ԭ�Ӳ�����X86��CMPXCHG���ָ�������ʵ��ʵ�ָ���������lock free�����ݽṹ��
CAS������C����ʵ�����£�

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
		cout << "���Ľ����count��" << count << endl;
	}

	{
		atomic_int count = 0;
		thread thread1(threadAtomicValue, ref(count));
		thread thread2(threadAtomicValue, ref(count));
		thread1.join();
		thread2.join();
		cout << "���Ľ����count��" << count << endl;
	}
	return 1;
}