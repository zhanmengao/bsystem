// DisguiseProcess_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DisguiseProcess.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == DisguiseProcess(5476, L"C:\\Windows\\explorer.exe", L"explorer.exe"))
	{
		printf("Dsisguise Process Error.\n");
	}
	printf("Dsisguise Process OK.\n");

	system("pause");
	return 0;
}

