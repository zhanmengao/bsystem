// EnumProcess_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "EnumInfo.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ��������
	if (FALSE == EnumProcess())
	{
		printf("Enum Process Error!\n");
	}

	system("pause");
	system("cls");

	// �����߳�
	if (FALSE == EnumThread())
	{
		printf("Enum Thread Error!\n");
	}

	system("pause");
	system("cls");

	// ����ָ������ģ��
	if (FALSE == EnumProcessModule(6876))
	{
		printf("Enum Process Module Error!\n");
	}

	system("pause");
	return 0;
}

