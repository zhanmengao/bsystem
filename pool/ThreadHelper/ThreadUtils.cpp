#include"ThreadUtils.h"

#ifdef _WIN32
#include "tlhelp32.h"
std::vector<ThreadHelper::ThreadInfo> ThreadHelper::GetThreadList(DWORD dwPID)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwPID);
	std::vector<ThreadInfo> vec;
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te = { sizeof(te) };
		BOOL bOK = Thread32First(hSnap, &te);
		for (; bOK; bOK = Thread32Next(hSnap, &te))
		{
			if (dwPID == 0 || te.th32OwnerProcessID == dwPID)
			{
				ThreadInfo info;
				info.pid = te.th32OwnerProcessID;
				info.tid = te.th32ThreadID;
				vec.push_back(info);
			}
		}
		CloseHandle(hSnap);
	}
	return vec;
}

ThreadHelper::ThreadTime ThreadHelper::GetThreadTime(HANDLE hThread)
{
	ThreadHelper::ThreadTime time = { 0 };
	GetThreadTimes(hThread, &time.CreateionTime, &time.ExitTime, &time.KernelTime, &time.UserTime);
	return time;
}
BOOL ThreadHelper::SetContext(HANDLE hThread, CONTEXT* pContext)
{
	::thread_suspend(hThread);
	pContext->ContextFlags = CONTEXT_CONTROL;
	BOOL bRet = SetThreadContext(hThread, pContext);
	::thread_resume(hThread);
	return bRet;
}
__int64 ThreadHelper::FileTimeToQuadWord(PFILETIME pft)
{
	return (Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
}
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
	*retv = static_cast<DWORD*>(&ret);
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