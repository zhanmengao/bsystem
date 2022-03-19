#include "Define.h"
#define STR_LEN 100  

unsigned WINAPI NumberOfA(void *arg);
unsigned WINAPI NumberOfOthers(void *arg);

static char str[STR_LEN];
static HANDLE hEvent;
/*
事件同步对象与前2种同步方法相比有很大不同，
区别在于：该方法下创建对象时，可以在自动non-signaled状态运行的auto-reset模式和与之相反的manual-reset模式中任选其一。
而事件对象的主要特点是可以创建manual-reset模式的对象。

传递事件对象句柄并希望改为non-signed状态时，应调用ResetEvent函数。如果希望改为signaled状态，则可以调用SetEvent函数。
*/
int mainEvent()
{
	HANDLE hThread1, hThread2;

	//以non-signaled创建manual-reset模式的事件对象  
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	while (1)
	{
		//fputs("Input string: ", stdout);
		fgets(str, STR_LEN, stdin);

		//读入字符串后改为signaled状态  
		SetEvent(hEvent);
	}
	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	//non-signaled 如果不更改，对象继续停留在signaled  
	ResetEvent(hEvent);
	CloseHandle(hEvent);

	return 0;
}

unsigned WINAPI NumberOfA(void *arg)
{
	int i, cnt = 0;
	while (1)
	{
		printf("\nWaitForSingleObject %d", GetCurrentThreadId());
		WaitForSingleObject(hEvent, INFINITE);
		ResetEvent(hEvent);
		printf("\nAwake %d", GetCurrentThreadId());
		for (i = 0; str[i] != 0; i++)
		{
			if (str[i] == 'A')
				cnt++;
		}
		printf("\nNum of A: %d \n", cnt);
	}
	return 0;
}

unsigned WINAPI NumberOfOthers(void *arg)
{
	int i, cnt = 0;
	while (1)
	{
		printf("\nWaitForSingleObject %d \n", GetCurrentThreadId());
		WaitForSingleObject(hEvent, INFINITE);
		ResetEvent(hEvent);
		printf("\n Awake %d", GetCurrentThreadId());
		for (i = 0; str[i] != 0; i++)
		{
			if (str[i] != 'A')
				cnt++;
		}
		printf("\n Num of others: %d", cnt - 1);
	}
	return 0;
}