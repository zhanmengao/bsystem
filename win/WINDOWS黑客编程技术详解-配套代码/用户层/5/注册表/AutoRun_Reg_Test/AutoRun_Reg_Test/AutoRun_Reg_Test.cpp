// AutoRun_Reg_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AutoRun_Reg.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ��һ�ַ�ʽ�޸�ע���:HKEY_CURRENT_USER
	if (FALSE == Reg_CurrentUser("C:\\Users\\DemonGan\\Desktop\\520_1.exe", "520"))
	{
		printf("Reg_CurrentUser Error!\n");
	}
	// �ڶ��ַ�ʽ�޸�ע���:HKEY_LOCAL_MACHINE
	if (FALSE == Reg_LocalMachine("C:\\Users\\DemonGan\\Desktop\\520_2.exe", "520"))
	{
		printf("Reg_LocalMachine Error!\n");
	}

	printf("Reg OK.\n");

	system("pause");
	return 0;
}

