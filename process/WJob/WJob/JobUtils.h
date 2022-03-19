#pragma once
#include<Windows.h>
#include"../../WindowsProcessHelper/CreateProcess/ProcessElevationUtils.h"
#include"../../../../C++space/C语言基础/C_指针训练/BitUtils.h"

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
	//设置优先级
	static void SetPriorityClass(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit,DWORD Priority = NORMAL_PRIORITY_CLASS)
	{
		limit.PriorityClass = Priority;
		limit.LimitFlags |= JOB_OBJECT_LIMIT_PRIORITY_CLASS;
	}
	//设置最大用户模式时间限制
	static void SetPerJobUserTimeLimit(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit, 
		LONGLONG max)
	{
		limit.PerJobUserTimeLimit.QuadPart = max;							//最大用户模式时间
		//limit.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;								//设置时扣除已终止运行运行的进程的CPU时间统计信息
		limit.LimitFlags |= JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME;				//设置时保留现有CPU时间限制
	}

	//设置相对时间量差
	static void SetSchedulingClass(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit,
		unsigned scheduling)
	{
		limit.SchedulingClass = scheduling;															//相对时间量差,默认5
		limit.LimitFlags |= JOB_OBJECT_LIMIT_SCHEDULING_CLASS;			
	}
	//设置不弹出异常对话框
	static void SetDieNoException(JOBOBJECT_BASIC_LIMIT_INFORMATION& limit)
	{
		limit.LimitFlags |= JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;
	}
	//设置新进程是否属于作业
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
	//限制作业中单个进程的最大内存
	static void SetProcessMemoryUser(JOBOBJECT_EXTENDED_LIMIT_INFORMATION& limit, SIZE_T max)
	{
		limit.PeakProcessMemoryUsed = max;
		limit.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
	}
	//限制作业所有进程的内存之和
	static void SetJobMemoryUser(JOBOBJECT_EXTENDED_LIMIT_INFORMATION& limit, SIZE_T max)
	{
		limit.PeakJobMemoryUsed = max;
		limit.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
	}
	
	//禁用ExitWindowEx注销、关机、重启或断开系统电源
	static void SetLimitExit(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
	}

	//禁止读取剪切板
	static void SetLimitReadClipboard(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD;
	}

	//禁止清除剪切板内容
	static void SetLimitWriteClipboard(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD;
	}

	//禁止SystemParametersInfo更改系统参数
	static void SetLimitSystemParametersInfo(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;
	}

	//禁止ChangeDisplaySettings更改显示设置
	static void SetLimitChangeDisplaySettings(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;
	}

	//禁止通过CreateDesktop或SwitchDesktop来创建或切换桌面
	static void SetLimitDesktop(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DESKTOP;
	}

	//禁止访问作业外部进程所创建的用户对象
	static void SetLimitHandles(JOBOBJECT_BASIC_UI_RESTRICTIONS& limit)
	{
		limit.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;
	}

	static BOOL SetLimitHandle(HANDLE hJob, HANDLE hObject, BOOL bGrant)
	{
		return UserHandleGrantAccess(hObject, hJob, bGrant);
	}
	//将进程添加到作业
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
	//杀死所有进程
	static BOOL KillProcess(HANDLE hJob, UINT uExitCode)
	{
		return TerminateJobObject(hJob, uExitCode);
	}

	//获取统计信息
	static _JOBOBJECT_BASIC_ACCOUNTING_INFORMATION GetAccounInfor(HANDLE hJob)
	{
		_JOBOBJECT_BASIC_ACCOUNTING_INFORMATION info = { 0 };
		QueryInformationJobObject(hJob, JobObjectBasicAccountingInformation, &info, sizeof(info), NULL);
		return info;
	}

	//获取基本统计和IO统计
	static _JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION GetAccounIOInfor(HANDLE hJob)
	{
		_JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION info = { 0 };
		QueryInformationJobObject(hJob, JobObjectBasicAndIoAccountingInformation, &info, sizeof(info), NULL);
		return info;
	}
	//获取进程ID集合
	void EnumProcessIDsInJob(HANDLE hJob,DWORD MAX_PROCESS_IDS, std::vector<DWORD>& vec)
	{
		//估算需要的内存
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