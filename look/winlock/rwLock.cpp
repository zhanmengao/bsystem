//������д������� ��д��SRWLock
#include <stdio.h>
#include <process.h>
#include <windows.h>

//���ÿ���̨�����ɫ
BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

const int READER_NUM = 5;  //���߸���
//�ؼ��κ��¼�
CRITICAL_SECTION g_cs;
SRWLOCK          g_srwLock;

//���߳��������(��κ�����ʵ��)
void ReaderPrintf(char *pszFormat, ...)
{
	va_list   pArgList;
	va_start(pArgList, pszFormat);
	EnterCriticalSection(&g_cs);
	vfprintf(stdout, pszFormat, pArgList);
	LeaveCriticalSection(&g_cs);
	va_end(pArgList);
}

//���̺߳���
unsigned int __stdcall ReaderThreadFun(PVOID pM)
{
	ReaderPrintf("     ���Ϊ%d�Ķ��߽���ȴ���...\n", GetCurrentThreadId());
	//���������ȡ�ļ�
	AcquireSRWLockShared(&g_srwLock);
	//��ȡ�ļ�
	ReaderPrintf("���Ϊ%d�Ķ��߿�ʼ��ȡ�ļ�...\n", GetCurrentThreadId());
	Sleep(rand() % 100);
	ReaderPrintf(" ���Ϊ%d�Ķ��߽�����ȡ�ļ�\n", GetCurrentThreadId());
	//���߽�����ȡ�ļ�
	ReleaseSRWLockShared(&g_srwLock);
	return 0;
}
//д���߳��������
void WriterPrintf(char *pszStr)
{
	EnterCriticalSection(&g_cs);
	SetConsoleColor(FOREGROUND_GREEN);
	printf("     %s\n", pszStr);
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	LeaveCriticalSection(&g_cs);
}
//д���̺߳���
unsigned int __stdcall WriterThreadFun(PVOID pM)
{
	WriterPrintf("д���߳̽���ȴ���...\n");
	//д������д�ļ�
	AcquireSRWLockExclusive(&g_srwLock);
	//д�ļ�
	WriterPrintf("  д�߿�ʼд�ļ�.....\n");
	Sleep(rand() % 100);
	WriterPrintf("  д�߽���д�ļ�\n");
	//���д�߽���д�ļ�
	ReleaseSRWLockExclusive(&g_srwLock);
	return 0;
}
int mainrwLock()
{
	printf("  ����д������� ��д��SRWLock\n");
	//��ʼ����д���͹ؼ���
	InitializeCriticalSection(&g_cs);
	InitializeSRWLock(&g_srwLock);
	HANDLE hThread[READER_NUM + 1];
	int i;
	//���������������߳�
	for (i = 1; i <= 2; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
	//����д���߳�
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadFun, NULL, 0, NULL);
	Sleep(50);
	//��������������߽��
	for (; i <= READER_NUM; i++)
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
	WaitForMultipleObjects(READER_NUM + 1, hThread, TRUE, INFINITE);
	for (i = 0; i < READER_NUM + 1; i++)
		CloseHandle(hThread[i]);
	//���ٹؼ���
	DeleteCriticalSection(&g_cs);
	return 0;
}