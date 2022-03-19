#pragma once
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <sstream>
/*
std::atomic_flag��һ��ԭ�ӵĲ������ͣ���֧������ԭ�Ӳ�����

  test_and_set, ���atomic_flag�������ã��򷵻�true; ���atomic_flag����δ�����ã�������֮������false
  clear. �����atomic_flag����
*/
std::atomic_flag lock = ATOMIC_FLAG_INIT;
std::stringstream stream;

void append_numer(int x)
{
	while (lock.test_and_set());
	stream << "thread " << x << "\n";
	lock.clear();
}

int mainAtomic_Flag()
{
	std::vector<std::thread> ths;
	for (int i = 0; i < 10; i++)
		ths.push_back(std::thread(append_numer, i));
	for (int i = 0; i < 10; i++)
		ths[i].join();
	std::cout << stream.str();
	return 0;
}