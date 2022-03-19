#include "Define.h"

/*
参数 bWaitAll 为 false，只要一个内核对象为已通知状态，则该函数返回。
*/
const unsigned int THREAD_NUM = 10;
DWORD WINAPI  ThreadFuncFalse(LPVOID p)
{
	int n = *(int*)p;
	Sleep(1000 * n); //第 n 个线程睡眠 n 秒
	printf("我是， pid = %d 的子线程\n", GetCurrentThreadId());   //输出子线程pid
	printf(" pid = %d 的子线程退出\n", GetCurrentThreadId());

	return 0;
}
int WaitForMultipleObjectsFalse()
{
	printf("我是主线程， pid = %d\n", GetCurrentThreadId());  //输出主线程pid
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFuncFalse, &i, 0, NULL); // 创建线程
	}

	WaitForMultipleObjects(THREAD_NUM, hThread, false, INFINITE);	//只要有一个线程返回就结束
	return 0;
}

DWORD WINAPI  ThreadFuncTrue(LPVOID p)
{
	int n = *(int*)p;
	Sleep(1000 * n);  //第 n 个线程睡眠 n 秒
	printf("我是， pid = %d 的子线程\t", GetCurrentThreadId());   //输出子线程pid
	printf(" pid = %d 的子线程退出\n\n", GetCurrentThreadId());   //延时10s后输出

	return 0;
}
int WaitForMultipleObjectsTrue()
{
	printf("我是主线程， pid = %d\n", GetCurrentThreadId());  //输出主线程pid
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFuncTrue, &i, 0, NULL); // 创建线程
	}

	WaitForMultipleObjects(THREAD_NUM, hThread, true, INFINITE);	//一直等待，直到所有子线程全部返回
	return 0;
}

