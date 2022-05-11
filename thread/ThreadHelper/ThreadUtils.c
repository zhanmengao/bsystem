//
// Created by zhanmengao on 10/20/20.
//
#include"ThreadUtils.h"
#ifdef _WIN32
DWORD pthread_self()
{
	return GetCurrentThreadId();
}
HANDLE GetThread()
{
	return GetCurrentThread();
}
DWORD thread_suspend(HANDLE hThread)
{
	return SuspendThread(hThread);
}
DWORD thread_resume(HANDLE hThread)
{
	return ResumeThread(hThread);
}
//线程安全:会给每个线程分配单独的ptd数据块，会有每个线程自己的errno等全局变量
HANDLE thread_create(_beginthreadex_proc_type proc, void* argv, BOOL bStart)
{
	DWORD dwCreateFlags = 0;
	if (!bStart)
	{
		dwCreateFlags = CREATE_SUSPENDED;
	}
	return (HANDLE)_beginthreadex(NULL,NULL,proc, argv, dwCreateFlags,NULL);
}
DWORD threadCreate(LPTHREAD_START_ROUTINE proc, LPVOID argv, BOOL bStart)
{
	DWORD dwCreateFlags = 0;
	if (!bStart)
	{
		dwCreateFlags = CREATE_SUSPENDED;
	}
	DWORD dwTid = 0;
	HANDLE hThread = CreateThread(NULL, 0, proc, argv, dwCreateFlags, &dwTid);
	CloseHandle(hThread);
	return dwTid;
}
DWORD pthread_join(HANDLE hThread, void** retv)
{
	DWORD waitRet = WaitForSingleObject(hThread, INFINITE);
	DWORD ret;
	GetExitCodeThread(hThread, &ret);
	*retv = ret;
	return waitRet;
}
void thread_exit(DWORD dwExit)
{
	_endthreadex(dwExit);
}
BOOL pthread_kill(HANDLE hThread, DWORD dwExit)
{
	return TerminateThread(hThread, dwExit);
}
BOOL pthread_alive(HANDLE hThread)
{
	DWORD dwExit = 0;
	BOOL bRet = GetExitCodeThread(hThread, &dwExit);
	if (!bRet)
	{
		return FALSE;
	}
	return dwExit == STILL_ACTIVE;
}
BOOL thread_time(HANDLE hThread, FILETIME* creat, FILETIME* exit, FILETIME* kernel, FILETIME* user)
{
	return GetThreadTimes(hThread, creat, exit, kernel, user);
}
BOOL pthread_switch()
{
	return SwitchToThread();
}
#else
pthread_t GetThreadID()
{
	return pthread_self();
}
pthread_t NewThread(void*(*start_routine)(void *), void* argv)
{
	pthread_t tid;
	pthread_create(&tid, NULL, start_routine, argv);
	return tid;
}
void ThreadExit(void *retval)
{
	pthread_exit(retval);
}

int ThreadCancel(pthread_t tid)
{
	return pthread_cancel(tid);
}
void ThreadTestCancel()
{
	pthread_testcancel();
}
int ThreadKill(pthread_t tid, int sig)
{
	return pthread_kill(tid, sig);
}
int pthread_alive(pthread_t tid)
{
	int kill_rc = pthread_kill(tid, 0);
	if (kill_rc == ESRCH)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int ThreadWait(pthread_t tid)
{
	int ret;
	pthread_join(tid, (void**)&ret);
	return ret;
}
int ThreadDetach(pthread_t tid)
{
	return pthread_detach(tid);
}
#endif