#include"OSThread.h"

struct THREAD_PAIR
{
    PTHREAD_START_ROUTINE proc;
    void* argv;
};
#ifdef __linux
void* PTHREAD_START(void* lpThreadParameter)
{
    THREAD_PAIR *pair = static_cast<THREAD_PAIR*>(lpThreadParameter);
    if(pair)
    {
        long ret = pair->proc(pair->argv);
        return (void*)ret;
    }
    return nullptr;
}
#elif _WIN32
unsigned __stdcall PTHREAD_START(void* lpThreadParameter)
{
	THREAD_PAIR *pair = static_cast<THREAD_PAIR*>(lpThreadParameter);
	if (pair)
	{
		long ret = pair->proc(pair->argv);
		return ret;
	}
	return 0;
}
#endif
void OSThread::Create(PTHREAD_START_ROUTINE proc, void* parameter)
{
	Clean();
    pair = new THREAD_PAIR();
    pair->proc = proc;
    pair->argv = parameter;
#ifdef _WIN32
	hThread = thread_create(PTHREAD_START, pair, false);
#elif __linux
#endif
}
void OSThread::Run()
{
#ifdef _WIN32
	if (hThread)
	{
		::thread_resume(hThread);
	}
#else
    pthread_create(&hThread,NULL,PTHREAD_START,pair);
#endif
}
void OSThread::Kill()
{
#ifdef __linux
	if (hThread!=NULL)
	{
		::pthread_kill(hThread, 9);
	}
#elif _WIN32
#endif
}
void OSThread::Wait()
{
	if (hThread != NULL)
	{
		void* ret;
		::pthread_join(hThread, &ret);
		dwRet = (DWORD)ret;
	}
}
DWORD OSThread::ExitCode()
{
	if (!Alive())
	{
		Wait();
	}
	return dwRet;
}
#ifdef _WIN32
ThreadHelper::ThreadTime OSThread::GetTime()
{
	return ThreadHelper::GetThreadTime(hThread);
}
CONTEXT& OSThread::GetContext()
{
	mContext.ContextFlags = CONTEXT_ALL;
	Pause();
	GetThreadContext(hThread, &mContext);
	Run();
	return mContext;
}
#endif
bool OSThread::Alive()
{
	if (hThread != NULL)
	{
		return ::pthread_alive(hThread);
	}
	return false;
}
PID OSThread::GetPID()
{
#ifdef _WIN32
	if (hThread!=NULL)
	{
		return GetThreadId(hThread);
	}
	return 0;
#elif __linux
	return hThread;
#endif
}
PID OSThread::GetPIDSelf()
{
	return pthread_self();
}

void OSThread::Clean()
{
	Kill();
	Wait();
	if (pair != nullptr)
	{
		delete pair;
		pair = nullptr;
	}
#ifdef _WIN32
	if (hThread != NULL)
	{
		CloseHandle(hThread);
		hThread = NULL;
	}
#endif
}
OSThread::~OSThread()
{
	Clean();
}
void OSThread::Sleep(uint64_t time)
{
#ifdef _WIN32
	::Sleep(time);
#else
    struct timespec s;
    s.tv_sec = time/1000;
    s.tv_nsec = (time%1000) *1000;
    nanosleep(&s,NULL);
#endif
}
#ifdef _WIN32
void OSThread::Pause()
{
	::thread_suspend(hThread);
}

BOOL OSThread::SetPriority(int nPriority)
{
	return SetThreadPriority(hThread, nPriority);
}
DWORD OSThread::GetPriority()
{
	return GetThreadPriority(hThread);
}
#endif
