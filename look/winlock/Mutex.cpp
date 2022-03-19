#include "Define.h"
#define NUM_THREAD 50  

unsigned WINAPI threadInc2(void * arg);
unsigned WINAPI threadDes2(void * arg);

long long num1 = 0;
HANDLE hMutex;
/*
������������������κ�ӵ���ߣ��򽫽���signaled״̬�����ø��ص����ͬ�������⣬�����������ں˶���
��������ĳһ�̻߳�ȡʱΪnon-signaled״̬���ͷ�ʱ����signaled״̬����ˣ���������WaitForSingleObject������֤�������Ƿ��ѷ��䡣
��������WaitForSingleObject��������ʱ�Զ�����non-signaled״̬����Ϊ���ǡ�auto-reset��ģʽ���ں˶���
*/
int mainMutex()
{
	HANDLE tHandles[NUM_THREAD];
	int i;
	hMutex = CreateMutex(NULL, FALSE, NULL);						//��������������ʱΪsignaled״̬  
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc2, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes2, NULL, 0, NULL);
	}
	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);
	CloseHandle(hMutex);															//���ٶ���  
	printf("result: %lld \n", num1);

	return 0;
}

unsigned WINAPI threadInc2(void * arg)
{
	int i;
	//�����ȴ���
	WaitForSingleObject(hMutex, INFINITE);  //��ȡ�������Կ��  
	for (i = 0; i < 50000000; i++)
	{
		num1 += 1;
	}
	printf("�������߳� ++�� pid = %d tickcount = %ld \n", GetCurrentThreadId(),num1);
	ReleaseMutex(hMutex);  //�ͷţ��뿪ʱ�Ͻ�Կ��  
	return 0;
}

unsigned WINAPI threadDes2(void * arg)
{
	int i;
	//�����ȴ���
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < 50000000; i++)
		num1 -= 1;
	printf("�������߳� -- �� pid = %d tickcount = %ld \n", GetCurrentThreadId(), num1);
	ReleaseMutex(hMutex);
	return 0;
}