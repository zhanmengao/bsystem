#pragma once
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <sstream>
/*
std::atomic_flag是一个原子的布尔类型，可支持两种原子操作：

  test_and_set, 如果atomic_flag对象被设置，则返回true; 如果atomic_flag对象未被设置，则设置之，返回false
  clear. 清清除atomic_flag对象
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