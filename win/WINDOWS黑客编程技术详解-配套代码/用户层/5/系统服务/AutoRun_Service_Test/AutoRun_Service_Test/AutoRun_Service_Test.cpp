// AutoRun_Service_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ServiceOperate.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;
	char szFileName[MAX_PATH] = "C:\\Users\\DemonGan\\Desktop\\AutoRun_Service_Test\\Debug\\ServiceTest.exe";

	// ��������������
	bRet = SystemServiceOperate(szFileName, 0);
	if (FALSE == bRet)
	{
		printf("Create Error!\n");
	}
	bRet = SystemServiceOperate(szFileName, 1);
	if (FALSE == bRet)
	{
		printf("Start Error!\n");
	}
	printf("Create and Start OK.\n");

	system("pause");

	// ֹͣ��ɾ������
	bRet = SystemServiceOperate(szFileName, 2);
	if (FALSE == bRet)
	{
		printf("Stop Error!\n");
	}
	bRet = SystemServiceOperate(szFileName, 3);
	if (FALSE == bRet)
	{
		printf("Delete Error!\n");
	}
	printf("Stop and Delete OK.\n");

	system("pause");
	return 0;
}

