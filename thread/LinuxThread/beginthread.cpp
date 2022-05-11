/* 创建第一个线程。主进程结束，则撤销线程。 */

#include "Define.h"

DWORD WINAPI ThreadFunc(LPVOID p)
{
	printf("我是子线程， pid = %d\n", GetCurrentThreadId());   //输出子线程pid
	return 0;
}
void __cdecl ThreadFunc2(void* p)
{
	printf("我是子线程， pid = %d p = %s\n", GetCurrentThreadId(),p);   //输出子线程pid
}
void mainCreate()
{
	HANDLE hThread;
	DWORD  threadId;

	hThread = CreateThread(NULL, 0, ThreadFunc, 0, 0, &threadId);						// 创建线程
	uintptr_t tid  = _beginthread(ThreadFunc2, 0, "QQQ");										// 创建线程
	printf("我是主线程， pid = %d\n", GetCurrentThreadId());									//输出主线程pid
	Sleep(2000);
}


