#include "Define.h"

/*
���� dwMilliseconds Ϊ0����ú�����������
*/
DWORD WINAPI  ThreadFunc0(LPVOID p)
{
	Sleep(10000); //˯��10��
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());   //������߳�pid

	return 0;
}
int WaitForSingleObject0()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadFunc0, 0, 0, NULL); // �����߳�
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());  //������߳�pid

	WaitForSingleObject(hThread, 0);	//���ȴ���ֱ�ӷ���
	return 0;
}

/*
���� dwMilliseconds Ϊ5000���ȴ�5��󷵻�
*/
DWORD WINAPI  ThreadFunc5000(LPVOID p)
{
	printf("���ǣ� pid = %d �����߳�\n", GetCurrentThreadId());   //������߳�pid
	Sleep(10000); //˯��10��
	printf(" pid = %d �����߳��˳�\n", GetCurrentThreadId());    //��ʱ10s�����

	return 0;
}

int WaitForSingleObject5000()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadFunc5000, 0, 0, NULL); // �����߳�
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());  //������߳�pid

	WaitForSingleObject(hThread, 5000);	//�ȴ�5s
	return 0;
}

/*
���� dwMilliseconds ΪINFINITE�����߳�һֱ������ֱ��hHandle��ָ��Ķ����Ϊ���ź�״̬ʱΪֹ
*/
DWORD WINAPI  ThreadFuncINFINITE(LPVOID p)
{
	printf("���ǣ� pid = %d �����߳�\n", GetCurrentThreadId());   //������߳�pid
	Sleep(10000); //˯��10��
	printf(" pid = %d �����߳��˳�\n", GetCurrentThreadId());   //��ʱ10s�����

	return 0;
}
int WaitForSingleObjectINFINITE()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadFuncINFINITE, 0, 0, NULL); // �����߳�
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());  //������߳�pid

	WaitForSingleObject(hThread, INFINITE);	//һֱ�ȴ���ֱ�����̷߳���
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


