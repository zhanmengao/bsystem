#pragma once
#include"atomic_value.h"
#include <iostream>
#include <thread>
#include<mutex>
#include<atomic>
using namespace std;
/*
template< class T >
bool atomic_compare_exchange_weak( std::atomic* obj,T* expected, T desired );
template< class T >
bool atomic_compare_exchange_weak( volatile std::atomic* obj,T* expected, T desired );

Fetch-And-Add��һ�������Ա�����+1��ԭ�Ӳ���
Test-and-set��дֵ��ĳ���ڴ�λ�ò��������ֵ
*/

void threadAtomic(std::atomic<int> &num)
{
	printf("begin time %ld \n", time(NULL));
	for (int i = 0; i < 100000; i++)
	{
		std::atomic_fetch_add<int>(&num, 1);
	}
	printf("end time %ld \n", time(NULL));
}

int mainAtomic()
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
		std::atomic<int> count = 0;
		thread thread1(threadAtomic, ref(count));
		thread thread2(threadAtomic, ref(count));
		thread1.join();
		thread2.join();
		cout << "���Ľ����count��" << count << endl;
	}
	return 1;
}