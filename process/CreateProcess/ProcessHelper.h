#pragma once
#include<Windows.h>
#include <WtsApi32.h>
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include<vector>
#include<map>
#include<memory>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include"../../../../C++space/LogManager/LogManager/LogManager.hpp"
using namespace std;

class ProcessInfo
{
public:
	DWORD   dwSize;
	DWORD   cntUsage;
	DWORD   th32ProcessID;          // this process
	ULONG_PTR th32DefaultHeapID;
	DWORD   th32ModuleID;           // associated exe
	DWORD   cntThreads;
	DWORD   th32ParentProcessID;    // this process's parent process
	LONG    pcPriClassBase;         // Base priority of process's threads
	DWORD   dwFlags;
	stdstr   szExeFile;    // Path
public:
	ProcessInfo();
	ProcessInfo(const ProcessInfo& other);
	ProcessInfo& operator=(const ProcessInfo& other);
	ProcessInfo(const PROCESSENTRY32& other);
	ProcessInfo& operator=(const PROCESSENTRY32& other);
};
class ProcessTree
{
public:
	ProcessInfo data;
	std::vector<ProcessTree> ChildList;
};
struct ProcessTimes
{
	_FILETIME CreationTime;
	_FILETIME ExitTime;
	_FILETIME KernelTime;
	_FILETIME UserTimes;
};
class ProcessHelper
{
public:
	static BOOL GetProcessList(std::map<DWORD, ProcessInfo> &_mapProcess,bool fullPath = false);
	static stdstr GetProcessName(DWORD nPid);
	static stdstr GetProcessUser();
	static DWORD GetProcessID(const TCHAR* pName);
	static DWORD GetProcessID();										//获取自己进程ID
	static void SuspendProcess(DWORD dwPID, BOOL bSuspend);				//暂停一个进程（暂停其下所有线程）

	static DWORD GetProcessExitCode(HANDLE hProcess);

	static stdstr GetProcessFullPath(DWORD dwPID);


	static HANDLE GetProcess(DWORD nID);					//通过进程ID获取进程句柄
	static HANDLE GetProcess();										//获取自己进程的伪句柄
	static DWORD GetProcessfromThread(HANDLE hThread);										//通过线程句柄获取进程ID
	static DWORD GetProcessfromParentID(DWORD dwParentProcessId, vector<ProcessInfo>& vec);
	static void GetProcessfromParentID(const std::map<DWORD, ProcessInfo> &_mapProcess,
		DWORD dwParentProcessId, vector<ProcessInfo>& vec);

	static void GetProcessTreefromParentID(const std::map<DWORD, ProcessInfo> &_mapProcess,
		DWORD dwParentProcessId, ProcessTree& tree);
	static DWORD GetParentID(DWORD pid);

	static BOOL GetTime(HANDLE hProcess, FILETIME* creat, FILETIME* exit, FILETIME* kernel, FILETIME* user);
	static ProcessTimes GetProcessTime(HANDLE hProcess);
	static BOOL SetPriority(HANDLE hProcess, DWORD fdwtPriority);
	static DWORD GetPriority(HANDLE hProcess);

	static IO_COUNTERS GetIOInfo(HANDLE hProcess);
private:
	static BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);
};
