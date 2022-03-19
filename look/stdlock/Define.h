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
	�����ܱ������ݵ�ָ������ô��ݵ�������������֮�⣬
	�����Ǻ�������ֵ�����Ǵ洢���ⲿ�ɼ��ڴ棬������Բ�������ʽ���ݵ��û��ṩ�ĺ�����ȥ��
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

//�̰߳�ȫ��ջ
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

//��������ͼ��һ���������������ڸò����ѱ��Ͳ����ʱ�������ǲ�����ġ�
class hierarchical_mutex
{
private:
	std::mutex internal_mutex;
	unsigned long const hierarchy_value;                       //��������ĵȼ�
	unsigned long previous_hierarchy_value;                   //��һ���������ݻ���                 
	static  unsigned long this_thread_hierarchy_value;           //��ǰ������static
	//��������𣬿��ܷ����
	void check_for_hierarchy_violation();
	//�����ɹ��󣬸��µ�ǰ������
	void update_hierarchy_value();
public:
	explicit hierarchical_mutex(unsigned long value) :
		hierarchy_value(value),
		previous_hierarchy_value(0)
	{}
	void lock();
	//��������ԭ������ǰ��������
	void unlock();
	//���Լ���
	bool try_lock();
};

