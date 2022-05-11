#ifndef _THREADUTILS_H
#define _THREADUTILS_H

#ifdef __cplusplus
#include<vector>
#endif
#ifdef _WIN32
#include<Windows.h>
#include<process.h>
#else
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include<signal.h>
#include<errno.h>
#endif
#ifdef WIN32
typedef HANDLE PTHREAD;
typedef DWORD PID;
#elif __linux
typedef long(*PTHREAD_START_ROUTINE)(void* lpThreadParameter);
typedef pthread_t PTHREAD;
typedef unsigned long       DWORD;
#endif
#ifdef _WIN32

DWORD pthread_self();
HANDLE GetThread();
DWORD threadCreate(LPTHREAD_START_ROUTINE proc, LPVOID argv, BOOL bStart);

HANDLE thread_create(_beginthreadex_proc_type proc, void* argv, BOOL bStart);
DWORD thread_suspend(HANDLE hThread);
DWORD thread_resume(HANDLE hThread);
DWORD pthread_join(HANDLE hThread,void** retv);
void thread_exit(DWORD dwExit);
BOOL pthread_kill(HANDLE hThread, DWORD dwExit);
BOOL pthread_alive(HANDLE hThread);
BOOL pthread_switch();

BOOL thread_time(HANDLE hThread, FILETIME* creat, FILETIME* exit, FILETIME* kernel, FILETIME* user);

#ifdef __cplusplus
class ThreadHelper
{
public:
	struct ThreadInfo
	{
		DWORD pid;
		DWORD tid;
	};
	struct ThreadTime
	{
		FILETIME CreateionTime;
		FILETIME ExitTime;
		FILETIME KernelTime;
		FILETIME UserTime;
	};
public:
	static std::vector<ThreadInfo> GetThreadList(DWORD dwPID);
	static ThreadTime GetThreadTime(HANDLE hThread);
	static BOOL SetContext(HANDLE hThread, CONTEXT* pContext);
private:
	static __int64 FileTimeToQuadWord(PFILETIME pft);
};
#endif

#else

typedef pthread_t PID;
pthread_t GetThreadID();
pthread_t NewThread(void*(*start_routine)(void *), void* argv);
void ThreadExit(void *retval);
void ThreadTestCancel();                 //Check Test
int ThreadCancel(pthread_t tid);
int ThreadKill(pthread_t tid, int sig);
int pthread_alive(pthread_t tid);
int ThreadWait(pthread_t tid);
int ThreadDetach(pthread_t tid);


#endif



#endif