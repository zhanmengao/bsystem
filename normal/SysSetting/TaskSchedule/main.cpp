#include"TaskSchduleManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TaskSchduleManager task;
	BOOL bRet = FALSE;

	// ���� ����ƻ�
	//bRet = task.NewTask(_T("520"), _T("C:\\Users\\DemonGan\\Desktop\\520.exe"), _T(""), _T("wby"));
	if (FALSE == bRet)
	{
		printf("Create Task Schedule Error!\n");
	}
	std::vector<_TaskSchduleInfo> vec;
	task.GetTaskList(vec);
	// ��ͣ
	printf("Create Task Schedule OK!\n");
	system("pause");

	// ж�� ����ƻ�
	bRet = task.Delete(_T("520"));
	if (FALSE == bRet)
	{
		printf("Delete Task Schedule Error!\n");
	}

	printf("Delete Task Schedule OK!\n");
	system("pause");
	return 0;
}