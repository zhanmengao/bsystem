// CreateMutex_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>


// �ж��Ƿ��ظ�����
BOOL IsAlreadyRun()
{
	HANDLE hMutex = NULL;
	hMutex = ::CreateMutex(NULL, FALSE, "TEST");
	if (hMutex)
	{
		if (ERROR_ALREADY_EXISTS == ::GetLastError())
		{
			return TRUE;
		}
	}
	return FALSE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	// �ж��Ƿ��ظ�����
	if (IsAlreadyRun())
	{
		printf("Already Run!!!!\n");
	}
	else
	{
		printf("NOT Already Run!\n");
	}

	system("pause");
	return 0;
}

