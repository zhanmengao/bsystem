#include "Define.h"
/*
��һ��������PRTL_CRITICAL_SECTION_DEBUG DebugInfo; ���Ե�ʱ���õģ��Ȳ������ܡ�
�ڶ���������LONG LockCount; ��ʼ��Ϊ-1��n��ʾ��n���߳��ڵȴ���
������������LONG RecursionCount; ��ʾ�ùؼ��ε�ӵ���̶߳Դ���Դ��ùؼ��δ�������Ϊ0��
���ĸ�������HANDLE OwningThread; ��ӵ�иùؼ��ε��߳̾��
�����������HANDLE LockSemaphore; ʵ������һ���Ը�λ�¼���
������������ULONG_PTR SpinCount; ��ת�������ã����ڶദ����
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
	InitializeCriticalSection(&cs);  //��ʼ���ٽ���
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			hHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			hHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, hHandles, TRUE, INFINITE);
	DeleteCriticalSection(&cs);  //�ͷ��ٽ���
	printf("result: %lld \n", num);

	return 0;
}

unsigned WINAPI threadInc(void *arg)
{
	int i;
	EnterCriticalSection(&cs);  //�����ٽ���
	for (i = 0; i < 50000000; i++)
		num += 1;
	LeaveCriticalSection(&cs);  //�뿪�ٽ���
	return 0;
}

unsigned WINAPI threadDes(void *arg)
{
	int i;
	EnterCriticalSection(&cs);  //�����ٽ���
	for (i = 0; i < 50000000; i++)
		num -= 1;
	LeaveCriticalSection(&cs);  //�뿪�ٽ���
	return 0;
}

//---------------------------------------------TEST 2-----------------------------------------------------

const unsigned int THREAD_NUM = 50;
unsigned int g_Count = 0;

DWORD WINAPI  ThreadFunc2(LPVOID p)
{
	Sleep(50);
	EnterCriticalSection(&cs);  // ����ؼ���
	g_Count++;
	LeaveCriticalSection(&cs);  // �뿪�ؼ���
	Sleep(50);

	return 0;
}
int mainCriticalSection1()
{
	InitializeCriticalSection(&cs);	// ��ʼ���ؼ���
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFunc2, 0, 0, NULL); // �����߳�
	}
	WaitForMultipleObjects(THREAD_NUM, hThread, true, INFINITE);    //һֱ�ȴ���ֱ���������߳�ȫ������
	printf(" �ܹ� %d ���̸߳� g_Count ��ֵ��һ������ g_Count = %d\n", THREAD_NUM, g_Count);
	DeleteCriticalSection(&cs);		//���ٹؼ���
	return 0;
}

