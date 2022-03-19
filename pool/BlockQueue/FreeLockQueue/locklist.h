#ifndef JZLOCKFREELIST_H
#define JZLOCKFREELIST_H
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include <stack>
#include <iostream>
#include <thread>
#include <list>
#include<memory>
#ifdef _WIN32
#include<Windows.h>
#else
#endif

using namespace std;

/*
��������
��ʵ������������������ƽʱ��˵���ֹ����ͱ�������
������һ�ֱ��۵Ĳ��ԣ���������Ϊÿ�η��ʹ�����Դ��ʱ���ܻᷢ����ͻ��������Ը�������ܣ�ʱ�䣩����֤���ݰ�ȫ��

������һ���ֹ۵Ĳ��ԣ��������̷߳��ʹ�����Դ���ᷢ����ͻ�����Բ���Ҫ����������߳̽�����ִ�У�����Ҫֹͣ��һ��������ͻ�������Ե�ǰ����ֱ��û�г�ͻΪֹ��
�����Ĳ���ʹ��һ�ֽ����ȽϽ����ļ�����CAS Compare And Swap���������̳߳�ͻ��һ����⵽��ͻ�����������Ե�ǰ����ֱ��û�г�ͻΪֹ��
CAS��ϵͳԭ�CAS������һ��CPU��ԭ��ָ����Բ������̰߳�ȫ���⡣
CAS ��α��:
template <class T>
bool CAS(T* addr, T expected, T value)
{
if (*addr == expected)
{
*addr = value;
return true;
}
return false;
}
*/

/*
CAS �� expected ��һ���ڴ��ַ���бȽϣ�����Ƚϳɹ����ͽ��ڴ������滻Ϊ new ��
��ǰ�������������Ӳ����ʵ��������������� Inter ����������������� CMPXCHG ��
��� CAS ��һ���������ԭ�Ӳ�����

GCC4.1+�汾��֧��CAS��ԭ�Ӳ���,API�ӿ����£�
**bool** __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)

type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
*/



#define usleep(ms) Sleep(ms/1000)
#define LOCK 0
#define UNLOCK 1
template <typename T>
class JzLockfreeList
{
private:
	std::list<T> list;

private:
	std::atomic<int> mutex;
	int lock = LOCK;
	int unlock = UNLOCK;
public:
	JzLockfreeList() :mutex(0){};
	~JzLockfreeList() {};

	void Lock()
	{
		while (!mutex.compare_exchange_weak(lock, UNLOCK))
		{
			usleep(100);
		}
	}

	void Unlock()
	{
		mutex.compare_exchange_weak(unlock, LOCK);
	}

	void Push(T data)
	{
		Lock();
		list.push_back(data);
		Unlock();
	}

	T Front()
	{
		Lock();
		T data = list.front();
		Unlock();

		return data;
	}

	void PopFront()
	{
		Lock();
		list.pop_front();
		Unlock();
	}

	bool IsEmpty()
	{
		Lock();
		if (list.empty())
		{
			Unlock();
			return true;
		}
		else
		{
			Unlock();
			return false;
		}
	}

	bool Find(T data)
	{
		typename std::list<T>::iterator it;

		Lock();
		for (it = list.begin(); it != list.end(); ++it)
		{
			if (*it == data)
			{
				Unlock();
				return true;
			}
		}

		Unlock();
		return false;
	}
};


#endif


/*
* @brief��compare & swap(CAS)���������expect��swap������ͷ���--�Ƿ񽻻��ɹ�, ע��expect�������ȣ���ѵ�ǰֵд�뵽expected���档
* �����strong��weak���ܻ����[spurious wakeup](<http://en.wikipedia.org/wiki/Spurious_wakeup>).
* @param          ��x����expect,������Ϊdesired ����true��
*                 ��������ֵд��expect������false

class atomic
{
bool compare_exchange_strong(T& expect,T desired)			//�����Ƚϵ�ֵ,  �������õ�ֵ
bool compare_exchange_weak(T& expect, T desired)
}
*/