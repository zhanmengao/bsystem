#include"PthreadPool.h"

//这里的函数禁止彼此调用
int PthreadPool::ThreadSize()
{
	TLock lock(thread_map_lock);
	return mThreadMap.size();
}
void PthreadPool::AddThread(int count)
{
	TLock lock(thread_map_lock);
	for (int i = 0; i < count && mThreadMap.size() < max_thr_num; i++)
	{
		thread *pThread = new thread(&PthreadPool::threadpool_thread, this);
		mThreadMap.insert(std::make_pair(pThread->get_id(), pThread));
		pThread->detach();
	}
}
void PthreadPool::DeleteThread(std::thread::id tid)
{
	TLock lock(thread_map_lock);
	printf("thread 0x%x is exiting\n", tid);
	auto it = mThreadMap.find(std::this_thread::get_id());
	if (it != mThreadMap.end())
	{
		delete it->second;
		mThreadMap.erase(it);
	}
}
void PthreadPool::IncreBusyThreadCount()
{
	TLock lock(thread_counter);
	printf("thread 0x%x start working\n", std::this_thread::get_id());
	busy_thr_num++;
}
void PthreadPool::DimiBusyThreadCount()
{
	TLock lock(thread_counter);
	printf("thread 0x%x end working\n", std::this_thread::get_id());
	busy_thr_num--;
}
int PthreadPool::GetWaiExitThrCount()
{
	TLock lock(wait_exit_num_lock);
	return wait_exit_thr_num;
}
void PthreadPool::SetWaiExitThrCount(int val)
{
	TLock lock(wait_exit_num_lock);
	wait_exit_thr_num = val;
}
void PthreadPool::DimiWaiExitThrCount()
{
	TLock lock(wait_exit_num_lock);
	wait_exit_thr_num--;
}

int PthreadPool::BusyThreadCount()
{
	TLock lock(thread_counter);
	return busy_thr_num;
}