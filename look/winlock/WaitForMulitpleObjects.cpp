#include "Define.h"

/*
���� bWaitAll Ϊ false��ֻҪһ���ں˶���Ϊ��֪ͨ״̬����ú������ء�
*/
const unsigned int THREAD_NUM = 10;
DWORD WINAPI  ThreadFuncFalse(LPVOID p)
{
	int n = *(int*)p;
	Sleep(1000 * n); //�� n ���߳�˯�� n ��
	printf("���ǣ� pid = %d �����߳�\n", GetCurrentThreadId());   //������߳�pid
	printf(" pid = %d �����߳��˳�\n", GetCurrentThreadId());

	return 0;
}
int WaitForMultipleObjectsFalse()
{
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());  //������߳�pid
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFuncFalse, &i, 0, NULL); // �����߳�
	}

	WaitForMultipleObjects(THREAD_NUM, hThread, false, INFINITE);	//ֻҪ��һ���̷߳��ؾͽ���
	return 0;
}

DWORD WINAPI  ThreadFuncTrue(LPVOID p)
{
	int n = *(int*)p;
	Sleep(1000 * n);  //�� n ���߳�˯�� n ��
	printf("���ǣ� pid = %d �����߳�\t", GetCurrentThreadId());   //������߳�pid
	printf(" pid = %d �����߳��˳�\n\n", GetCurrentThreadId());   //��ʱ10s�����

	return 0;
}
int WaitForMultipleObjectsTrue()
{
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());  //������߳�pid
	HANDLE hThread[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
	{
		hThread[i] = CreateThread(NULL, 0, ThreadFuncTrue, &i, 0, NULL); // �����߳�
	}

	WaitForMultipleObjects(THREAD_NUM, hThread, true, INFINITE);	//һֱ�ȴ���ֱ���������߳�ȫ������
	return 0;
}

