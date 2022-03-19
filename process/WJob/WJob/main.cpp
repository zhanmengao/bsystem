#include"JobUtils.h"
void StartRestrictedProcess()
{
	BOOL bInJob = FALSE;
	IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
	if (bInJob)
	{
		MessageBox(NULL, TEXT("�����Ѿ���һ����ҵ����"), TEXT("IsProcessInJob"), MB_OK);
		return;
	}

	//����һ����ҵ�ں˶���
	HANDLE hJob = CreateJobObject(NULL, TEXT("Wintellect_RestrictedProcessJob"));

	//��������
	JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };
	jobli.PriorityClass = IDLE_PRIORITY_CLASS;										//����ֻ�������ڿ������ȼ�
	jobli.PerJobUserTimeLimit.QuadPart = 10000;									//����ʹ�ô���1���cpuʱ��
	jobli.LimitFlags = JOB_OBJECT_LIMIT_PRIORITY_CLASS | JOB_OBJECT_LIMIT_JOB_TIME;					//�������Ƶ�����
	SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &jobli, sizeof(jobli));					//����ҵ��������

	JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir = { 0 };
	jobuir.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
}