#include "Define.h"
#define STR_LEN 100  

unsigned WINAPI NumberOfA(void *arg);
unsigned WINAPI NumberOfOthers(void *arg);

static char str[STR_LEN];
static HANDLE hEvent;
/*
�¼�ͬ��������ǰ2��ͬ����������кܴ�ͬ��
�������ڣ��÷����´�������ʱ���������Զ�non-signaled״̬���е�auto-resetģʽ����֮�෴��manual-resetģʽ����ѡ��һ��
���¼��������Ҫ�ص��ǿ��Դ���manual-resetģʽ�Ķ���

�����¼���������ϣ����Ϊnon-signed״̬ʱ��Ӧ����ResetEvent���������ϣ����Ϊsignaled״̬������Ե���SetEvent������
*/
int mainEvent()
{
	HANDLE hThread1, hThread2;

	//��non-signaled����manual-resetģʽ���¼�����  
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	while (1)
	{
		//fputs("Input string: ", stdout);
		fgets(str, STR_LEN, stdin);

		//�����ַ������Ϊsignaled״̬  
		SetEvent(hEvent);
	}
	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	//non-signaled ��������ģ��������ͣ����signaled  
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