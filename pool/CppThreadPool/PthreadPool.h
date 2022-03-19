#pragma once
#include<thread>
#include<map>
#include <condition_variable>
#include <mutex>
#include <deque>
#include<functional>
using namespace std;
#define DEFAULT_TIME 5						/*10s检测一次*/
#define MIN_WAIT_TASK_NUM 10            /*如果queue_size > MIN_WAIT_TASK_NUM 添加新的线程到线程池*/ 
#define DEFAULT_THREAD_VARY 10          /*每次创建和销毁线程的个数*/

class PthreadPool
{
	typedef struct
	{
		std::function<void*(void*)> function;				/* 函数指针，回调函数 */       
		void *arg;									 /* 上面函数的参数 */
	} threadpool_task_t;                    /* 各子线程任务结构体 */
public:
	PthreadPool(int min_thr_num = 5, int max_thr_num = 50, int queue_max_size = -1, bool bQuickExit = false);
	//添加任务
	int threadpool_add(std::function<void*(void*)> function, void *arg);

	//析构系统资源，并释放内存
	int threadpool_destroy();
private:
	//管理线程主函数
	void adjust_thread();
	//工作线程主函数
	void threadpool_thread();
private:
	//线程信息
	const int min_thr_num;                    /* 线程池最小线程数 */
	const int max_thr_num;                    /* 线程池最大线程数 */
	thread *adjustThread;						 /* 存管理线程tid */
	int shutdown;									 /* 标志位，线程池使用状态，true或false */
	const bool bQuickExit;					//是否立刻退出
	bool bRecvTask;								//是否还接收任务
private:
	//线程管理 实现
	std::mutex thread_map_lock;							/* 线程Map锁 */
	map<thread::id, thread*> mThreadMap;
	std::mutex wait_exit_num_lock;					/* 要销毁的线程个数锁 */
	int wait_exit_thr_num;									 /* 要销毁的线程个数 */
	std::mutex thread_counter;							/* 记录忙状态线程个数琐 -- busy_thr_num */
	int busy_thr_num;										/* 忙状态线程个数 */
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
	//阻塞队列实现
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