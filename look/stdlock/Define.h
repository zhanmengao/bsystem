#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <windows.h>
#include <memory>
#include <assert.h>
#include <stack>
#include <iostream>
#include <mutex>
using namespace std;


void MutexMain();
int UniqueMutexMain();
int Main_unique_lock();
void SafeMain();

void mainLock();

void MainCallOnce();
void MainOnceFlag();

void MainCond();

class MutexTest
{
public:
	MutexTest() : m_mutex(), m_charStack() { }
	~MutexTest() { }
	void Push(int n, char c);
	/*
	切勿将受保护数据的指针或引用传递到互斥锁作用域之外，
	无论是函数返回值，还是存储在外部可见内存，亦或是以参数的形式传递到用户提供的函数中去。
	*/
	template<typename Function>
	void process(Function func)
	{
		std::lock_guard<std::mutex> guard (m_mutex);
		func(m_charStack);
	}
private:
	std::mutex       m_mutex;
	std::stack<char> m_charStack;
};

//线程安全的栈
template<typename T>
class Stack
{
private:
	std::stack<T>      m_data;
	mutable std::mutex m_mutex;
public:
	Stack() : m_data(std::stack<int>()){}
	Stack(const Stack& other)
	{
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}
	Stack& operator=(const Stack&) = delete;
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) nullptr;
		const std::shared_ptr<T> res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) return nullptr;
		value = data.top();
		data.pop();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

//当代码试图对一个互斥量上锁，在该层锁已被低层持有时，上锁是不允许的。
class hierarchical_mutex
{
private:
	std::mutex internal_mutex;
	unsigned long const hierarchy_value;                       //我这把锁的等级
	unsigned long previous_hierarchy_value;                   //上一把锁的数据缓存                 
	static  unsigned long this_thread_hierarchy_value;           //当前锁级别static
	//检查锁级别，看能否加锁
	void check_for_hierarchy_violation();
	//加锁成功后，更新当前锁数据
	void update_hierarchy_value();
public:
	explicit hierarchical_mutex(unsigned long value) :
		hierarchy_value(value),
		previous_hierarchy_value(0)
	{}
	void lock();
	//解锁，还原到加锁前的锁数据
	void unlock();
	//尝试加锁
	bool try_lock();
};

