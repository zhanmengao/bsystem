// AutoRun_Task_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MyTaskSchedule.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CMyTaskSchedule task;
	BOOL bRet = FALSE;

	// ���� ����ƻ�
	bRet = task.NewTask("520", "C:\\Users\\DemonGan\\Desktop\\520.exe", "", "");
	if (FALSE == bRet)
	{
		printf("Create Task Schedule Error!\n");
	}

	// ��ͣ
	printf("Create Task Schedule OK!\n");
	system("pause");

	// ж�� ����ƻ�
	bRet = task.Delete("520");
	if (FALSE == bRet)
	{
		printf("Delete Task Schedule Error!\n");
	}

	printf("Delete Task Schedule OK!\n");
	system("pause");
	return 0;
}

