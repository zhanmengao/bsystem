#include"PthreadPool.h"
PthreadPool::PthreadPool(int min_thr_num, int max_thr_num, int queue_max_size,bool bQuickExit) :
	min_thr_num(min_thr_num), max_thr_num(max_thr_num), m_maxCapacity(queue_max_size),
	busy_thr_num(0), shutdown(false), wait_exit_thr_num(0),bQuickExit(bQuickExit), bRecvTask(true)
{
	do
	{
		/* 启动 min_thr_num 个 work thread */
		AddThread(min_thr_num);
		adjustThread = new thread(&PthreadPool::adjust_thread, this);/* 启动管理者线程 */
		return;
	} while (false);
	threadpool_destroy();
	exit(-1);
}
//管理线程主函数
void PthreadPool::adjust_thread()
{
	int queue_size;
	int live_thr_num;
	int busy_thr_num;
	while (!shutdown)
	{
		std::this_thread::sleep_for(std::chrono::seconds(DEFAULT_TIME));        /*定时 对线程池管理*/
		if (shutdown)
		{
			return;
		}
		queue_size = TaskSize();                      /* 关注 任务数 */
		live_thr_num = ThreadSize();                  /* 存活 线程数 */
		busy_thr_num = BusyThreadCount();
		/* 创建新线程 算法： 任务数大于最小线程池个数, 且存活的线程数少于最大线程个数时
		//如：30>=10 && 40<100*/
		if ((queue_size >= MIN_WAIT_TASK_NUM && live_thr_num < max_thr_num) ||
			(live_thr_num == busy_thr_num))
		{
			/*一次增加 DEFAULT_THREAD 个线程*/
			AddThread(DEFAULT_THREAD_VARY);
		}

		/* 销毁多余的空闲线程 算法：忙线程X2 小于 存活的线程数 且 存活的线程数 大于 最小线程数时*/
		if ((busy_thr_num * 2) < live_thr_num  &&  live_thr_num > min_thr_num)
		{
			/* 一次销毁DEFAULT_THREAD个线程, 隨機10個即可 */
			SetWaiExitThrCount(DEFAULT_THREAD_VARY);
			/* 通知处在空闲状态的线程, 他们会自行终止*/
			Signal2Empty();
		}
	}
}
//工作线程主函数
void PthreadPool::threadpool_thread()
{
	threadpool_task_t task;
	while (true)
	{
		{
			/*刚创建出线程，等待任务队列里有任务，否则阻塞等待任务队列里有任务后再唤醒接收任务*/
			TLock lock(m_mutex);
			/*queue_size == 0 说明没有任务，调 wait 阻塞在条件变量上, 若有任务，跳过该while*/
			while (m_list.empty() && (!shutdown))
			{
				printf("thread 0x%x is waiting\n", std::this_thread::get_id());
				m_notEmpty.wait(lock);
				/*清除指定数目的空闲线程，如果要结束的线程个数大于0，结束线程*/
				if (GetWaiExitThrCount() > 0)
				{
					DimiWaiExitThrCount();
					/*如果线程池里线程个数大于最小值时可以结束当前线程*/
					if (ThreadSize() > min_thr_num)
					{
						printf("thread 0x%x is exiting\n", std::this_thread::get_id());
						mThreadMap.erase(std::this_thread::get_id());
						return;
					}
				}
			}
			/*如果指定了true，要关闭线程池里的每个线程，自行退出处理*/
			if (shutdown)
			{
				lock.unlock();
				printf("thread 0x%x is exiting\n", std::this_thread::get_id());
				return;
			}
			/*从任务队列里获取任务, 是一个出队操作*/
			task = *m_list.begin();
			m_list.pop_front();
			m_notFull.notify_all();
		}
		if (task.function != nullptr)
		{
			/*执行任务*/
			IncreBusyThreadCount();
			((task.function))(task.arg);                                           /*执行回调函数任务*/
			/*任务结束处理*/
			DimiBusyThreadCount();
		}
	}
}
//析构系统资源，并释放内存
int PthreadPool::threadpool_destroy()
{
	if (shutdown|| !bRecvTask)
	{
		return -1;
	}
	bRecvTask = false;					//不再接收新任务
	//看是否需要立刻退出，如果不需要立刻退出则等待线程处理完所有消息
	if (!bQuickExit)
	{
		TLock lock(m_mutex);
		//等待每个Task处理完后，查询还有多少Task
		while (!m_list.empty())
		{
			m_notFull.wait(lock);
		}
	}
	shutdown = true;
	/*先销毁管理线程*/
	if (adjustThread != nullptr && adjustThread->joinable())
	{
		adjustThread->join();
		delete adjustThread;
	}
	Signal2Empty();
	for (auto it = mThreadMap.begin(); it != mThreadMap.end(); it++)
	{
		if (it->second->joinable())
		{
			it->second->join();
		}
		delete it->second;
	}
	return 0;
}
