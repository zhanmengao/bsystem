#pragma once
#include<Windows.h>
#include"../../WindowsProcessHelper/CreateProcess/ProcessElevationUtils.h"
#include"../../../../C++space/C���Ի���/C_ָ��ѵ��/BitUtils.h"

class JobUtils
{
public:
	static HANDLE CreateJob(const TCHAR* jobName)
	{
		HANDLE hJob = CreateJobObject(NULL, jobName);
		return hJob;
	}
	static HANDLE OpenJob(const TCHAR* jobName, DWORD dwAcess, BOOL bInheritHandle)
	{
		return OpenJobObject(dwAcess, bInheritHandle, jobName);
	}

	static JOBOBJECT_BASIC_LIMIT_INFORMATION GetBaseLimit()
	{
		JOBOBJECT_BASIC_LIMIT_INFORMATION limit = { 0 };
		return limit;
	}
	static BOOL SetLimit(HANDLE hJob, JOBOBJECT_BASIC_LIMIT_INFORMATION& limit)
	{
		return SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &limit, sizeof(limit));
	}
	static JOBOBJECT_EXTENDED_LIMIT_INFORMATION GeExtendedLimit()
	{
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit = { 0 };
		return limit;
	}
	static JOBOBJECT_BASIC_UI_RESTRICTIONS GeRestrictionsLimit()
	{
		JOBOBJECT_BASIC_UI_RESTRICTIONS limit = { 0 };
		return limit;
	}
	static JOBOBJECT_SECURITY_LIMIT_INFORMATION GeSecurityLimit()
	{
		JOBOBJECT_SECURITY_LIMIT_INFORMATION limit = { 0 };
		return limit;
	}
	//�������ȼ�
	static void SetPriorityClass(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit,DWORD Priority = NORMAL_PRIORITY_CLASS)
	{
		limit.PriorityClass = Priority;
		limit.LimitFlags |= JOB_OBJECT_LIMIT_PRIORITY_CLASS;
	}
	//��������û�ģʽʱ������
	static void SetPerJobUserTimeLimit(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit, 
		LONGLONG max)
	{
		limit.PerJobUserTimeLimit.QuadPart = max;							//����û�ģʽʱ��
		//limit.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;								//����ʱ�۳�����ֹ�������еĽ��̵�CPUʱ��ͳ����Ϣ
		limit.LimitFlags |= JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME;				//����ʱ��������CPUʱ������
	}

	//�������ʱ������
	static void SetSchedulingClass(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit,
		unsigned scheduling)
	{
		limit.SchedulingClass = scheduling;															//���ʱ������,Ĭ��5
		limit.LimitFlags |= JOB_OBJECT_LIMIT_SCHEDULING_CLASS;			
	}
	//���ò������쳣�Ի���
	static void SetDieNoException(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit)
	{
		limit.LimitFlags |= JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;
	}
	//�����½����Ƿ�������ҵ
	static void SetProcessBreakway(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit, bool bLimit)
	{
		if (bLimit)
		{
			AddBits64(&limit.LimitFlags, JOB_OBJECT_LIMIT_BREAKAWAY_OK);
		}
		else
		{
			RemoveBits64(&limit.LimitFlags, JOB_OBJECT_LIMIT_BREAKAWAY_OK);
		}
	}
	//������ҵ�е������̵�����ڴ�
	static void SetProcessMemoryUser(JOBOBJECT_EXTENDED_LIMIT_INFORMATION& limit, SIZE_T max)
	{
		limit.PeakProcessMemoryUsed = max;
		limit.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
	}
	//������ҵ���н��̵��ڴ�֮��
	static void SetJobMemoryUser(JOBOBJECT_EXTENDED_LIMIT_INFORMATION& limit, SIZE_T max)
	{
		limit.PeakJobMemoryUsed = max;
		limit.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	}
	
	//����ExitWindowExע�����ػ���������Ͽ�ϵͳ��Դ
	static void SetLimitExit(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
	}

	//��ֹ��ȡ���а�
	static void SetLimitReadClipboard(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD;
	}

	//��ֹ������а�����
	static void SetLimitWriteClipboard(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD;
	}

	//��ֹSystemParametersInfo����ϵͳ����
	static void SetLimitSystemParametersInfo(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;
	}

	//��ֹChangeDisplaySettings������ʾ����
	static void SetLimitChangeDisplaySettings(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;
	}

	//��ֹͨ��CreateDesktop��SwitchDesktop���������л�����
	static void SetLimitDesktop(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DESKTOP;
	}

	//��ֹ������ҵ�ⲿ�������������û�����
	static void SetLimitHandles(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;
	}

	static BOOL SetLimitHandle(HANDLE hJob, HANDLE hObject, BOOL bGrant)
	{
		return UserHandleGrantAccess(hObject, hJob, bGrant);
	}
	//��������ӵ���ҵ
	static BOOL Add2Job(HANDLE hJob, HANDLE hProcess)
	{
		return AssignProcessToJobObject(hJob, hProcess);
	}
	static BOOL InJob(HANDLE hJob,HANDLE hProcess)
	{
		BOOL bInJob = FALSE;
		IsProcessInJob(hProcess, hJob,&bInJob);
		return bInJob;
	}
	//ɱ�����н���
	static BOOL KillProcess(HANDLE hJob, UINT uExitCode)
	{
		return TerminateJobObject(hJob, uExitCode);
	}

	//��ȡͳ����Ϣ
	static _JOBOBJECT_BASIC_ACCOUNTING_INFORMATION GetAccounInfor(HANDLE hJob)
	{
		_JOBOBJECT_BASIC_ACCOUNTING_INFORMATION info = { 0 };
		QueryInformationJobObject(hJob, JobObjectBasicAccountingInformation, &info, sizeof(info), NULL);
		return info;
	}

	//��ȡ����ͳ�ƺ�IOͳ��
	static _JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION GetAccounIOInfor(HANDLE hJob)
	{
		_JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION info = { 0 };
		QueryInformationJobObject(hJob, JobObjectBasicAndIoAccountingInformation, &info, sizeof(info), NULL);
		return info;
	}
	//��ȡ����ID����
	void EnumProcessIDsInJob(HANDLE hJob,DWORD MAX_PROCESS_IDS, std::vector<DWORD>& vec)
	{
		//������Ҫ���ڴ�
		DWORD cb = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + (MAX_PROCESS_IDS - 1) * sizeof(DWORD);
		PJOBOBJECT_BASIC_PROCESS_ID_LIST pjobpil = (PJOBOBJECT_BASIC_PROCESS_ID_LIST)_alloca(cb);

		pjobpil->NumberOfAssignedProcesses = MAX_PROCESS_IDS;
		QueryInformationJobObject(hJob, JobObjectBasicProcessIdList, pjobpil, cb, &cb);
		for (DWORD i = 0; i < pjobpil->NumberOfProcessIdsInList; i++)
		{
			vec.push_back(pjobpil->ProcessIdList[i]);
		}
		free(pjobpil);
	}
};