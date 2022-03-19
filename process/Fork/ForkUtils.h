#pragma once
#include<Windows.h>
#include <WtsApi32.h>
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include<vector>
#include<map>
#include<memory>
#include<WinBase.h>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include"../../../../C++space/LogManager/LogManager/LogManager.hpp"

//CREATE_SUSPENDED		启动后直接挂起
//CREATE_BREAKAWAY_FROM_JOB 创建后的进程不会进入父进程作业
class ForkUtils
{
public:
	static BOOL CreateEXE(const TCHAR* path, TCHAR* command, STARTUPINFO& si, PROCESS_INFORMATION& pi,
		DWORD dwFlag = NULL, BOOL bInherHandle = false, 
		SECURITY_ATTRIBUTES* psa = nullptr, SECURITY_ATTRIBUTES *tsa = nullptr);

	static BOOL CreateEXE(const TCHAR* path, TCHAR* command, STARTUPINFOEX& si, PROCESS_INFORMATION& pi,
		DWORD dwFlag = NULL, BOOL bInherHandle = false, 
		SECURITY_ATTRIBUTES* psa = nullptr, SECURITY_ATTRIBUTES *tsa = nullptr);

	static DWORD CreateEXE(const HANDLE& hToken, const TCHAR* exePath, STARTUPINFO& six, PROCESS_INFORMATION& pi,
		LogManager *log = nullptr,DWORD priority = NORMAL_PRIORITY_CLASS);
	static bool Exec(const char* path, UINT uiCmdShow = SW_HIDE);
	static BOOL ShellExec(const stdstr& cmdStr, const stdstr& Parameters, bool bWait = false);
	//op默认open
	static bool Shell(const TCHAR* path, const TCHAR* parameter,
		const TCHAR* op = NULL, UINT uiCmdShow = SW_HIDE,
		HWND hParent = NULL,const TCHAR* workdir = NULL);
	static BOOL OpenURL(const TCHAR* url);

	static void KillProcess(UINT exitCode = 0);
	static BOOL KillProcess(DWORD nProcessID,UINT exitCode = 0);
	static void KillProcess(const stdstr& processName);
	static DWORD Wait(HANDLE hProcess, bool bBlock);



	static STARTUPINFO GetStartInfo(bool bExtendParent = true);
	static void SetStartPos(STARTUPINFO &si, DWORD x, DWORD y, DWORD w, DWORD h);
	static void SetStartConsoleTitle(STARTUPINFO &si, TCHAR* title);
	static void SetStartWindowMax(STARTUPINFO &si, bool bMax);
	static void SetStartShowWindow(STARTUPINFO &si, bool bShow);
	static void SetStartIO(STARTUPINFO &si, HANDLE input, HANDLE output, HANDLE nError);

	static STARTUPINFOEX MallocStartInfoEX(bool bExtendParent = true);
	static void FreeStartInfoEX(STARTUPINFOEX& esi);
	static BOOL SetStartParent(STARTUPINFOEX &esi,HANDLE hParent);
	static BOOL SetStartHandleList(STARTUPINFOEX &esi, HANDLE *hHandle,size_t nCount);			//从新的父进程继承这些句柄

	static BOOL UACWhiteListOpen(const TCHAR* path);
};