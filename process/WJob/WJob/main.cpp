#include"JobUtils.h"
void StartRestrictedProcess()
{
	BOOL bInJob = FALSE;
	IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
	if (bInJob)
	{
		MessageBox(NULL, TEXT("程序已经在一个作业中了"), TEXT("IsProcessInJob"), MB_OK);
		return;
	}

	//创建一个作业内核对象
	HANDLE hJob = CreateJobObject(NULL, TEXT("Wintellect_RestrictedProcessJob"));

	//基本限制
	JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };
	jobli.PriorityClass = IDLE_PRIORITY_CLASS;										//进程只能运行在空闲优先级
	jobli.PerJobUserTimeLimit.QuadPart = 10000;									//不能使用大于1秒的cpu时间
	jobli.LimitFlags = JOB_OBJECT_LIMIT_PRIORITY_CLASS | JOB_OBJECT_LIMIT_JOB_TIME;					//设置限制的类型
	SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &jobli, sizeof(jobli));					//向作业加以限制

	JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir = { 0 };
	jobuir.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
}