#include"PthreadPool.h"
PthreadPool::PthreadPool(int min_thr_num, int max_thr_num, int queue_max_size,bool bQuickExit) :
	min_thr_num(min_thr_num), max_thr_num(max_thr_num), m_maxCapacity(queue_max_size),
	busy_thr_num(0), shutdown(false), wait_exit_thr_num(0),bQuickExit(bQuickExit), bRecvTask(true)
{
	do
	{
		/* ���� min_thr_num �� work thread */
		AddThread(min_thr_num);
		adjustThread = new thread(&PthreadPool::adjust_thread, this);/* �����������߳� */
		return;
	} while (false);
	threadpool_destroy();
	exit(-1);
}
//�����߳�������
void PthreadPool::adjust_thread()
{
	int queue_size;
	int live_thr_num;
	int busy_thr_num;
	while (!shutdown)
	{
		std::this_thread::sleep_for(std::chrono::seconds(DEFAULT_TIME));        /*��ʱ ���̳߳ع���*/
		if (shutdown)
		{
			return;
		}
		queue_size = TaskSize();                      /* ��ע ������ */
		live_thr_num = ThreadSize();                  /* ��� �߳��� */
		busy_thr_num = BusyThreadCount();
		/* �������߳� �㷨�� ������������С�̳߳ظ���, �Ҵ����߳�����������̸߳���ʱ
		//�磺30>=10 && 40<100*/
		if ((queue_size >= MIN_WAIT_TASK_NUM && live_thr_num < max_thr_num) ||
			(live_thr_num == busy_thr_num))
		{
			/*һ������ DEFAULT_THREAD ���߳�*/
			AddThread(DEFAULT_THREAD_VARY);
		}

		/* ���ٶ���Ŀ����߳� �㷨��æ�߳�X2 С�� �����߳��� �� �����߳��� ���� ��С�߳���ʱ*/
		if ((busy_thr_num * 2) < live_thr_num  &&  live_thr_num > min_thr_num)
		{
			/* һ������DEFAULT_THREAD���߳�, �S�C10������ */
			SetWaiExitThrCount(DEFAULT_THREAD_VARY);
			/* ֪ͨ���ڿ���״̬���߳�, ���ǻ�������ֹ*/
			Signal2Empty();
		}
	}
}
//�����߳�������
void PthreadPool::threadpool_thread()
{
	threadpool_task_t task;
	while (true)
	{
		{
			/*�մ������̣߳��ȴ���������������񣬷��������ȴ������������������ٻ��ѽ�������*/
			TLock lock(m_mutex);
			/*queue_size == 0 ˵��û�����񣬵� wait ����������������, ��������������while*/
			while (m_list.empty() && (!shutdown))
			{
				printf("thread 0x%x is waiting\n", std::this_thread::get_id());
				m_notEmpty.wait(lock);
				/*���ָ����Ŀ�Ŀ����̣߳����Ҫ�������̸߳�������0�������߳�*/
				if (GetWaiExitThrCount() > 0)
				{
					DimiWaiExitThrCount();
					/*����̳߳����̸߳���������Сֵʱ���Խ�����ǰ�߳�*/
					if (ThreadSize() > min_thr_num)
					{
						printf("thread 0x%x is exiting\n", std::this_thread::get_id());
						mThreadMap.erase(std::this_thread::get_id());
						return;
					}
				}
			}
			/*���ָ����true��Ҫ�ر��̳߳����ÿ���̣߳������˳�����*/
			if (shutdown)
			{
				lock.unlock();
				printf("thread 0x%x is exiting\n", std::this_thread::get_id());
				return;
			}
			/*������������ȡ����, ��һ�����Ӳ���*/
			task = *m_list.begin();
			m_list.pop_front();
			m_notFull.notify_all();
		}
		if (task.function != nullptr)
		{
			/*ִ������*/
			IncreBusyThreadCount();
			((task.function))(task.arg);                                           /*ִ�лص���������*/
			/*�����������*/
			DimiBusyThreadCount();
		}
	}
}
//����ϵͳ��Դ�����ͷ��ڴ�
int PthreadPool::threadpool_destroy()
{
	if (shutdown|| !bRecvTask)
	{
		return -1;
	}
	bRecvTask = false;					//���ٽ���������
	//���Ƿ���Ҫ�����˳����������Ҫ�����˳���ȴ��̴߳�����������Ϣ
	if (!bQuickExit)
	{
		TLock lock(m_mutex);
		//�ȴ�ÿ��Task������󣬲�ѯ���ж���Task
		while (!m_list.empty())
		{
			m_notFull.wait(lock);
		}
	}
	shutdown = true;
	/*�����ٹ����߳�*/
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
