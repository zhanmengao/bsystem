#include"PthreadPool.h"
//�������
int PthreadPool::threadpool_add(std::function<void*(void*)> function, void *arg)
{
	//�̳߳��ѹرգ�Unlock
	if (!bRecvTask)
	{
		return -1;
	}
	threadpool_task_t task;
	task.function = function;
	task.arg = arg;
	TLock lock(m_mutex);
	/* ==Ϊ�棬�����Ѿ����� ��wait���� */
	if (true == hasCapacity())
	{
		while (m_list.size() >= m_maxCapacity)
		{
			m_notFull.wait(lock);
		}
	}
	//�̳߳��ѹرգ�Unlock
	if (shutdown)
	{
		lock.unlock();
		return -1;
	}
	else
	{
		m_list.push_back(task);
		m_notEmpty.notify_all();
		return 0;
	}
}
size_t PthreadPool::TaskSize()
{
	TLock lock(m_mutex);
	return m_list.size();
}
bool PthreadPool::hasCapacity() const
{
	return m_maxCapacity > 0;
}
bool PthreadPool::TaskEmpty()
{
	TLock lock(m_mutex);
	return m_list.empty();
}
bool PthreadPool::TaskFull()
{
	if (false == hasCapacity())
	{
		return false;
	}
	TLock lock(m_mutex);
	return m_list.size() >= m_maxCapacity;
}
void PthreadPool::Signal2Empty()
{
	m_notEmpty.notify_all();
}