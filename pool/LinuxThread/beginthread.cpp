/* ������һ���̡߳������̽����������̡߳� */

#include "Define.h"

DWORD WINAPI ThreadFunc(LPVOID p)
{
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());   //������߳�pid
	return 0;
}
void __cdecl ThreadFunc2(void* p)
{
	printf("�������̣߳� pid = %d p = %s\n", GetCurrentThreadId(),p);   //������߳�pid
}
void mainCreate()
{
	HANDLE hThread;
	DWORD  threadId;

	hThread = CreateThread(NULL, 0, ThreadFunc, 0, 0, &threadId);						// �����߳�
	uintptr_t tid  = _beginthread(ThreadFunc2, 0, "QQQ");										// �����߳�
	printf("�������̣߳� pid = %d\n", GetCurrentThreadId());									//������߳�pid
	Sleep(2000);
}


