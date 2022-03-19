#pragma once
#include<thread>
#include<map>
#include <condition_variable>
#include <mutex>
#include <deque>
#include<functional>
using namespace std;
#define DEFAULT_TIME 5						/*10s���һ��*/
#define MIN_WAIT_TASK_NUM 10            /*���queue_size > MIN_WAIT_TASK_NUM ����µ��̵߳��̳߳�*/ 
#define DEFAULT_THREAD_VARY 10          /*ÿ�δ����������̵߳ĸ���*/

class PthreadPool
{
	typedef struct
	{
		std::function<void*(void*)> function;				/* ����ָ�룬�ص����� */       
		void *arg;									 /* ���溯���Ĳ��� */
	} threadpool_task_t;                    /* �����߳�����ṹ�� */
public:
	PthreadPool(int min_thr_num = 5, int max_thr_num = 50, int queue_max_size = -1, bool bQuickExit = false);
	//�������
	int threadpool_add(std::function<void*(void*)> function, void *arg);

	//����ϵͳ��Դ�����ͷ��ڴ�
	int threadpool_destroy();
private:
	//�����߳�������
	void adjust_thread();
	//�����߳�������
	void threadpool_thread();
private:
	//�߳���Ϣ
	const int min_thr_num;                    /* �̳߳���С�߳��� */
	const int max_thr_num;                    /* �̳߳�����߳��� */
	thread *adjustThread;						 /* ������߳�tid */
	int shutdown;									 /* ��־λ���̳߳�ʹ��״̬��true��false */
	const bool bQuickExit;					//�Ƿ������˳�
	bool bRecvTask;								//�Ƿ񻹽�������
private:
	//�̹߳��� ʵ��
	std::mutex thread_map_lock;							/* �߳�Map�� */
	map<thread::id, thread*> mThreadMap;
	std::mutex wait_exit_num_lock;					/* Ҫ���ٵ��̸߳����� */
	int wait_exit_thr_num;									 /* Ҫ���ٵ��̸߳��� */
	std::mutex thread_counter;							/* ��¼æ״̬�̸߳����� -- busy_thr_num */
	int busy_thr_num;										/* æ״̬�̸߳��� */
private:
	int ThreadSize();
	void AddThread(int count);
	void DeleteThread(std::thread::id tid);
	void IncreBusyThreadCount();
	void DimiBusyThreadCount();
private:
	int GetWaiExitThrCount();
	void SetWaiExitThrCount(int val);
	void DimiWaiExitThrCount();
public:
	int BusyThreadCount();
private:
	//��������ʵ��
	size_t TaskSize();
	bool hasCapacity() const;
	bool TaskEmpty();
	bool TaskFull();
	void Signal2Empty();
private:
	typedef std::unique_lock<std::mutex> TLock;
	typedef std::deque<threadpool_task_t> TList;
	TList m_list;

	const int m_maxCapacity;

	std::mutex m_mutex;
	std::condition_variable m_notEmpty;
	std::condition_variable m_notFull;
};