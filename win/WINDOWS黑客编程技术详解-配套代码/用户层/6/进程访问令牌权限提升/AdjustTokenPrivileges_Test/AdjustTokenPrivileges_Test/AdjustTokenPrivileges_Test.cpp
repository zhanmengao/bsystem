// AdjustTokenPrivileges_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AdjustTokenPrivilegesTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// �޸ĵ�ǰ�������Ʒ���Ȩ��
	if (FALSE == EnbalePrivileges(::GetCurrentProcess(), SE_DEBUG_NAME))
	{
		printf("Enable Privileges Error!\n");
	}

	printf("Enable Privileges OK!\n");
	system("pause");
	return 0;
}

