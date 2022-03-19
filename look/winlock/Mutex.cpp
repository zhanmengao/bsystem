#include "Define.h"
#define NUM_THREAD 50  

unsigned WINAPI threadInc2(void * arg);
unsigned WINAPI threadDes2(void * arg);

long long num1 = 0;
HANDLE hMutex;
/*
如果互斥量对象不属于任何拥有者，则将进入signaled状态，利用该特点进行同步。另外，互斥量属于内核对象，
互斥量被某一线程获取时为non-signaled状态，释放时进入signaled状态。因此，可以利用WaitForSingleObject函数验证互斥量是否已分配。
互斥量在WaitForSingleObject函数返回时自动进入non-signaled状态，因为它是“auto-reset”模式的内核对象。
*/
int mainMutex()
{
	HANDLE tHandles[NUM_THREAD];
	int i;
	hMutex = CreateMutex(NULL, FALSE, NULL);						//创建互斥量，此时为signaled状态  
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc2, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes2, NULL, 0, NULL);
	}
	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	CloseHandle(hMutex);															//销毁对象  
	printf("result: %lld \n", num1);

	return 0;
}

unsigned WINAPI threadInc2(void * arg)
{
	int i;
	//阻塞等待锁
	WaitForSingleObject(hMutex, INFINITE);  //获取，进入的钥匙  
	for (i = 0; i < 50000000; i++)
	{
		num1 += 1;
	}
	printf("我是子线程 ++， pid = %d tickcount = %ld \n", GetCurrentThreadId(),num1);
	ReleaseMutex(hMutex);  //释放，离开时上交钥匙  
	return 0;
}

unsigned WINAPI threadDes2(void * arg)
{
	int i;
	//阻塞等待锁
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < 50000000; i++)
		num1 -= 1;
	printf("我是子线程 -- ， pid = %d tickcount = %ld \n", GetCurrentThreadId(), num1);
	ReleaseMutex(hMutex);
	return 0;
}