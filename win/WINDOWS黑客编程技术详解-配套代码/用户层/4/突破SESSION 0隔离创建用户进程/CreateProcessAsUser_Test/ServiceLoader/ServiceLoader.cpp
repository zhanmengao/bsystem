// ServiceLoader.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ServiceOperate.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;
	char szExePath[] = "C:\\Users\\DemonGan\\Desktop\\CreateProcessAsUser_Test\\Debug\\CreateProcessAsUser_Test.exe";

	// ���ط���
	bRet = SystemServiceOperate(szExePath, 0);
	if (bRet)
	{
		printf("INSTALL OK.\n");
	}
	else
	{
		printf("INSTALL ERROR.\n");
	}
	// ��������
	bRet = SystemServiceOperate(szExePath, 1);
	if (bRet)
	{
		printf("START OK.\n");
	}
	else
	{
		printf("START ERROR.\n");
	}


	system("pause");

	// ֹͣ����
	bRet = SystemServiceOperate(szExePath, 2);
	if (bRet)
	{
		printf("STOP OK.\n");
	}
	else
	{
		printf("STOP ERROR.\n");
	}
	// ж�ط���
	bRet = SystemServiceOperate(szExePath, 3);
	if (bRet)
	{
		printf("UNINSTALL OK.\n");
	}
	else
	{
		printf("UNINSTALL ERROR.\n");
	}

	return 0;
}

