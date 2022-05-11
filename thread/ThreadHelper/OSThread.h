#ifndef _OSTHREAD_H
#define _OSTHREAD_H
#include<stdint.h>
#include"ThreadUtils.h"





class OSThread
{
private:
    struct THREAD_PAIR
    {
        PTHREAD_START_ROUTINE proc;
        void* argv;
    };
public:
	~OSThread();
public:
	void Create(PTHREAD_START_ROUTINE proc, void* parameter);
	void Run();
	void Pause();
	void Kill();
	void Wait();
	DWORD ExitCode();
public:
	PID GetPID();
	bool Alive();
public:
	static PID GetPIDSelf();
	static void Sleep(uint64_t time);
#ifdef _WIN32
public:
	CONTEXT& GetContext();
	ThreadHelper::ThreadTime GetTime();
	BOOL SetPriority(int nPriority);
	DWORD GetPriority();
#else
#endif
private:
	void Clean();
	
private:
	PTHREAD hThread = NULL;
    THREAD_PAIR *pair = nullptr;
	DWORD dwRet;
#ifdef _WIN32
	CONTEXT mContext = { 0 };
#else
#endif
};
#endif

#ifndef _WIN32
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
typedef struct _SECURITY_ATTRIBUTES
{
	long nLength;
	void* lpSecurityDescriptor;
	bool bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
class winpr_thread
{
	bool started;
	int pipe_fd[2];
	bool mainProcess;
	bool detached;
	bool joined;
	bool exited;
	long dwExitCode;
	pthread_t thread;
	size_t dwStackSize;
	void* lpParameter;
	pthread_mutex_t mutex;
	pthread_mutex_t threadIsReadyMutex;
	pthread_cond_t threadIsReady;
	LPTHREAD_START_ROUTINE lpStartAddress;
	LPSECURITY_ATTRIBUTES lpThreadAttributes;
#if defined(WITH_DEBUG_THREADS)
	void* create_stack;
	void* exit_stack;
#endif
};
#endif