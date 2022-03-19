#include "Define.h"

/*
参数 dwMilliseconds 为0，则该函数立即返回
*/
DWORD WINAPI  ThreadFunc0(LPVOID p)
{
	Sleep(10000); //睡眠10秒
	printf("我是子线程， pid = %d\n", GetCurrentThreadId());   //输出子线程pid

	return 0;
}
int WaitForSingleObject0()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadFunc0, 0, 0, NULL); // 创建线程
	printf("我是主线程， pid = %d\n", GetCurrentThreadId());  //输出主线程pid

	WaitForSingleObject(hThread, 0);	//不等待，直接返回
	return 0;
}

/*
参数 dwMilliseconds 为5000，等待5秒后返回
*/
DWORD WINAPI  ThreadFunc5000(LPVOID p)
{
	printf("我是， pid = %d 的子线程\n", GetCurrentThreadId());   //输出子线程pid
	Sleep(10000); //睡眠10秒
	printf(" pid = %d 的子线程退出\n", GetCurrentThreadId());    //延时10s后输出

	return 0;
}

int WaitForSingleObject5000()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadFunc5000, 0, 0, NULL); // 创建线程
	printf("我是主线程， pid = %d\n", GetCurrentThreadId());  //输出主线程pid

	WaitForSingleObject(hThread, 5000);	//等待5s
	return 0;
}

/*
参数 dwMilliseconds 为INFINITE，则线程一直被挂起，直到hHandle所指向的对象变为有信号状态时为止
*/
DWORD WINAPI  ThreadFuncINFINITE(LPVOID p)
{
	printf("我是， pid = %d 的子线程\n", GetCurrentThreadId());   //输出子线程pid
	Sleep(10000); //睡眠10秒
	printf(" pid = %d 的子线程退出\n", GetCurrentThreadId());   //延时10s后输出

	return 0;
}
int WaitForSingleObjectINFINITE()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadFuncINFINITE, 0, 0, NULL); // 创建线程
	printf("我是主线程， pid = %d\n", GetCurrentThreadId());  //输出主线程pid

	WaitForSingleObject(hThread, INFINITE);	//一直等待，直到子线程返回
	return 0;
}

/*
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
	int val;
	char buf[128];
}exit_t;


void* routineMain_Unsafe(void* data)
{
	exit_t retval;
	retval.val = 9;
	strcpy(retval.buf, "Un Safe Ret!");
	//pthread_exit(retval);
}

void* routineMain_Safe(void* data)
{
	exit_t *retval = (exit_t*)data;
	retval->val = 9;
	strcpy(retval->buf, "Safe Ret!");
	pthread_exit((void*)9);
}


int main(int argc, char const *argv[])
{
	pthread_t thread_safe;
	int ret;

	exit_t *safe_ret = (exit_t *)malloc(sizeof(exit_t));
	pthread_create(&thread_safe, NULL, routineMain_Safe, (void*)safe_ret);
	pthread_join(thread_safe, (void**)&ret);

	printf("safe_ret: val = %u,buf= %s ,ret= %u \n", safe_ret->val, safe_ret->buf, ret);
	if (safe_ret != NULL)
	{
		free(safe_ret);
		safe_ret = NULL;
	}
}
*/


