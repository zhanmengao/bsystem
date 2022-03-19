#include"ProcessHelper.h"
using namespace std;
#include <Psapi.h>
#include <wtypes.h>
#pragma comment (lib,"Psapi.lib")
#pragma comment(lib, "UserEnv.lib")
BOOL ProcessHelper::GetProcessList(std::map<DWORD, ProcessInfo> &_mapProcess,bool fullPath)
{
	_mapProcess.clear();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	BOOL bResult = Process32First(hProcessSnap, &pe32);//开始遍历

	while (bResult)
	{
		ProcessInfo info(pe32);
		if (fullPath)
		{
			stdstr str = GetProcessFullPath(pe32.th32ProcessID);
			if (!str.empty())
			{
				info.szExeFile = str;
			}
		}
		_mapProcess.insert(std::make_pair(pe32.th32ProcessID, info));						//字典存储
		bResult = Process32Next(hProcessSnap, &pe32);										//返回值为false，表明进程遍历完
	}
	CloseHandle(hProcessSnap);
	return TRUE;
}


stdstr ProcessHelper::GetProcessName(DWORD nPid)
{
	std::map<DWORD, ProcessInfo> mapProcess;
	GetProcessList(mapProcess);
	for (auto it = mapProcess.begin(); it != mapProcess.end(); it++)
	{
		if (it->first == nPid)
		{
			return it->second.szExeFile;
		}
	}
	return _T("");
}
HANDLE ProcessHelper::GetProcess()
{
	return GetCurrentProcess();
}
HANDLE ProcessHelper::GetProcess(DWORD nID)//通过进程ID获取进程句柄
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, nID);
}
DWORD ProcessHelper::GetProcessID()										//获取自己进程ID
{
	return GetCurrentProcessId();
}

DWORD ProcessHelper::GetProcessExitCode(HANDLE hProcess)
{
	DWORD dwRet = 0;
	GetExitCodeProcess(hProcess, &dwRet);
	return dwRet;
}
DWORD ProcessHelper::GetProcessID(const TCHAR* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe))
	{
		if (lstrcmp(pe.szExeFile, pName) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}
DWORD ProcessHelper::GetProcessfromThread(HANDLE hThread)
{
	return GetProcessIdOfThread(hThread);
}
/**
* 通过父进程查找子进程
**/
DWORD ProcessHelper::GetProcessfromParentID(DWORD dwParentProcessId, vector<ProcessInfo>& vec)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	BOOL bProcess = Process32First(hProcessSnap, &pe);

	while (bProcess)
	{
		if (pe.th32ParentProcessID == dwParentProcessId)
		{
			ProcessInfo info(pe);
			vec.push_back(info);
		}
		bProcess = Process32Next(hProcessSnap, &pe);
	}

	CloseHandle(hProcessSnap);

	return bProcess;
}
void ProcessHelper::GetProcessfromParentID(const std::map<DWORD, ProcessInfo> &_mapProcess,
	DWORD dwParentProcessId, vector<ProcessInfo>& vec)
{
	for (auto it = _mapProcess.begin(); it != _mapProcess.end(); it++)
	{
		if (it->second.th32ParentProcessID == dwParentProcessId)
		{
			vec.push_back(it->second);
		}
	}
}
void ProcessHelper::GetProcessTreefromParentID(const std::map<DWORD, ProcessInfo> &_mapProcess,
	DWORD dwParentProcessId, ProcessTree& tree)
{
	for (auto it = _mapProcess.begin(); it != _mapProcess.end(); it++)
	{
		if (it->second.th32ParentProcessID == dwParentProcessId)
		{
			ProcessTree info;
			info.data = it->second;
			//递归
			GetProcessTreefromParentID(_mapProcess,info.data.th32ProcessID,info);
			tree.ChildList.push_back(info);
		}
	}
}
DWORD ProcessHelper::GetParentID(DWORD pid)
{
#define ProcessBasicInformation 0  

	typedef struct
	{
		DWORD ExitStatus;
		DWORD PebBaseAddress;
		DWORD AffinityMask;
		DWORD BasePriority;
		ULONG UniqueProcessId;
		ULONG InheritedFromUniqueProcessId;
	}   PROCESS_BASIC_INFORMATION;
	typedef LONG(__stdcall *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);
	LONG                        status;
	DWORD                       dwParentPID = (DWORD)-1;
	HANDLE                      hProcess;
	PROCESS_BASIC_INFORMATION   pbi;

	PROCNTQSIP NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
		GetModuleHandle(_T("ntdll")), "NtQueryInformationProcess");

	if (NULL == NtQueryInformationProcess)
	{
		return (DWORD)-1;
	}
	// Get process handle
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (!hProcess)
	{
		return (DWORD)-1;
	}

	// Retrieve information
	status = NtQueryInformationProcess(hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL
	);

	// Copy parent Id on success
	if (!status)
	{
		dwParentPID = pbi.InheritedFromUniqueProcessId;
	}

	CloseHandle(hProcess);

	return dwParentPID;
}

stdstr ProcessHelper::GetProcessUser()
{
	TCHAR szChar[MAX_PATH];
	DWORD size;
	GetUserName(szChar, &size);
	return szChar;
}
BOOL ProcessHelper::GetTime(HANDLE hProcess, FILETIME* creat, FILETIME* exit, FILETIME* kernel, FILETIME* user)
{
	return GetProcessTimes(hProcess, creat, exit, kernel, user);
}
BOOL ProcessHelper::DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR            szDriveStr[500];
	TCHAR            szDrive[3];
	TCHAR            szDevName[100];
	INT                cchDevName;
	INT                i;

	//检查参数
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//获取本地磁盘字符串
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备名
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中
			{
				lstrcpy(pszNtPath, szDrive);//复制驱动器
				lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径
				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}
//获取进程完整路径
stdstr ProcessHelper::GetProcessFullPath(DWORD dwPID)
{
	TCHAR        szImagePath[MAX_PATH];
	TCHAR pszFullPath[MAX_PATH] = { 0 };
	HANDLE        hProcess = NULL;
	do
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
		if (!hProcess)
		{
			break;
		}
		if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
		{
			break;
		}
		if (!DosPathToNtPath(szImagePath, pszFullPath))
		{
			break;
		}
	} while (false);
	if (hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}	
	return pszFullPath;
}
void ProcessHelper::SuspendProcess(DWORD dwPID, BOOL bSuspend)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwPID);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te = { sizeof(te) };
		BOOL bOK = Thread32First(hSnap, &te);
		for (; bOK; bOK = Thread32Next(hSnap, &te))
		{
			if (te.th32OwnerProcessID == dwPID)
			{
				HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
				if (hThread != NULL)
				{
					if (bSuspend)
					{
						SuspendThread(hThread);
					}
					else
					{
						ResumeThread(hThread);
					}
					CloseHandle(hThread);
				}
			}
		}
		CloseHandle(hSnap);
	}
}
ProcessTimes ProcessHelper::GetProcessTime(HANDLE hProcess)
{
	ProcessTimes times = { 0 };
	BOOL bRet = GetProcessTimes(hProcess,
		&times.CreationTime, &times.ExitTime, &times.KernelTime, &times.UserTimes);
	return times;
}

IO_COUNTERS ProcessHelper::GetIOInfo(HANDLE hProcess)
{
	IO_COUNTERS info = { 0 };
	GetProcessIoCounters(hProcess, &info);
	return info;
}

BOOL ProcessHelper::SetPriority(HANDLE hProcess, DWORD fdwtPriority)
{
	return SetPriorityClass(hProcess, fdwtPriority);
}
DWORD ProcessHelper::GetPriority(HANDLE hProcess)
{
	//REALTIME_PRIORITY_CLASS
	//HIGH_PRIORITY_CLASS
	//ABOVE_NORMAL_PRIORITY_CLASS
	//NORMAL_PRIORITY_CLASS
	//BELOW_NORMAL_PRIORITY_CLASS
	//IDLE_PRIORITY_CLASS
	return GetPriorityClass(hProcess);
}

ProcessInfo::ProcessInfo()
{

}
ProcessInfo::ProcessInfo(const ProcessInfo& other) :szExeFile(other.szExeFile), dwSize(other.dwSize), th32ProcessID(other.th32ProcessID),
th32DefaultHeapID(other.th32DefaultHeapID), th32ModuleID(other.th32ModuleID), cntThreads(other.cntThreads),
th32ParentProcessID(other.th32ParentProcessID), pcPriClassBase(other.pcPriClassBase), cntUsage(other.cntUsage), dwFlags(other.dwFlags)
{}
ProcessInfo& ProcessInfo::operator=(const ProcessInfo& other)
{
	dwSize = other.dwSize;
	cntUsage = other.cntUsage;
	th32ProcessID = other.th32ProcessID;
	th32DefaultHeapID = other.th32DefaultHeapID;
	th32ModuleID = other.th32ModuleID;
	cntThreads = other.cntThreads;
	th32ParentProcessID = other.th32ParentProcessID;
	pcPriClassBase = other.pcPriClassBase;
	dwFlags = other.dwFlags;
	szExeFile = other.szExeFile;
	return *this;
}
ProcessInfo::ProcessInfo(const PROCESSENTRY32& other) :szExeFile(other.szExeFile), dwSize(other.dwSize), th32ProcessID(other.th32ProcessID),
th32DefaultHeapID(other.th32DefaultHeapID), th32ModuleID(other.th32ModuleID), cntThreads(other.cntThreads),
th32ParentProcessID(other.th32ParentProcessID), pcPriClassBase(other.pcPriClassBase), cntUsage(other.cntUsage), dwFlags(other.dwFlags)
{}
ProcessInfo& ProcessInfo::operator=(const PROCESSENTRY32& other)
{
	dwSize = other.dwSize;
	cntUsage = other.cntUsage;
	th32ProcessID = other.th32ProcessID;
	th32DefaultHeapID = other.th32DefaultHeapID;
	th32ModuleID = other.th32ModuleID;
	cntThreads = other.cntThreads;
	th32ParentProcessID = other.th32ParentProcessID;
	pcPriClassBase = other.pcPriClassBase;
	dwFlags = other.dwFlags;
	szExeFile = other.szExeFile;
	return *this;
}

