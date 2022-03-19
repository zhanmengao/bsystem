#include "Define.h"
/*
第一个参数：PRTL_CRITICAL_SECTION_DEBUG DebugInfo; 调试的时候用的，先不做介绍。
第二个参数：LONG LockCount; 初始化为-1，n表示有n个线程在等待。
第三个参数：LONG RecursionCount; 表示该关键段的拥有线程对此资源获得关键段次数，初为0。
第四个参数：HANDLE OwningThread; 即拥有该关键段的线程句柄
第五个参数：HANDLE LockSemaphore; 实际上是一个自复位事件。
第六个参数：ULONG_PTR SpinCount; 旋转锁的设置，用于多处理器
*/

#define NUM_THREAD 50
unsigned WINAPI threadInc(void *arg);
unsigned WINAPI threadDes(void *arg);
long long num = 0;
CRITICAL_SECTION cs;

int mainCriticalSection2()
{
	HANDLE hHandles[NUM_THREAD];
	int i;
	InitializeCriticalSection(&cs);  //初始化临界区
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			hHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			hHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, hHandles, TRUE, INFINITE);
	DeleteCriticalSection(&cs);  //释放临界区
	printf("result: %lld \n", num);

	return 0;
}

unsigned WINAPI threadInc(void *arg)
{
	int i;
	EnterCriticalSection(&cs);  //进入临界区
	for (i = 0; i < 50000000; i++)
		num += 1;
	LeaveCriticalSection(&cs);  //离开临界区
	return 0;
}

unsigned WINAPI threadDes(void *arg)
{
	int i;
	EnterCriticalSection(&cs);  //进入临界区
	for (i = 0; i < 50000000; i++)
		num -= 1;
	LeaveCriticalSection(&cs);  //离开临界区
	return 0;
}

//---------------------------------------------TEST 2-----------------------------------------------------

const unsigned int THREAD_NUM = 50;
unsigned int g_Count = 0;

DWORD WINAPI  ThreadFunc2(LPVOID p)
{
	Sleep(50);
	EnterCriticalSection(&cs);  // 进入关键段
	g_Count++;
	LeaveCriticalSection(&cs);  // 离开关键段
	Sleep(50);

	return 0;
}
int mainCriticalSection1()
{
	InitializeCriticalSection(&cs);	// 初始化关键段
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFunc2, 0, 0, NULL); // 创建线程
	}
	WaitForMultipleObjects(THREAD_NUM, hThread, true, INFINITE);    //一直等待，直到所有子线程全部返回
	printf(" 总共 %d 个线程给 g_Count 的值加一，现在 g_Count = %d\n", THREAD_NUM, g_Count);
	DeleteCriticalSection(&cs);		//销毁关键段
	return 0;
}

