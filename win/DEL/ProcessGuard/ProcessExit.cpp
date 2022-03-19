#include "stdafx.h"
#include "ProcessExit.h"
#include <string>
#include <shlwapi.h>
#include <Wtsapi32.h>
#include <Winternl.h>

#pragma comment(lib,"Wtsapi32.lib")
#pragma comment(lib, "Shlwapi.lib")

using namespace std;



typedef LONG(NTAPI *NTQueryInformationProcess)(
	IN  HANDLE ProcessHandle,
	IN  PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN  ULONG ProcessInformationLength,
	OUT PULONG ReturnLength    OPTIONAL);

NTQueryInformationProcess gNtQueryInformationProcess = NULL;

HMODULE hNtDll = NULL;

HMODULE LoadNTDLLFunctions()
{

	hNtDll = LoadLibrary(TEXT("ntdll.dll"));
	if (hNtDll == NULL) return NULL;

	gNtQueryInformationProcess = (NTQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");

	if (gNtQueryInformationProcess == NULL)
	{
		FreeLibrary(hNtDll);
		return NULL;
	}

	return hNtDll;
}

void FreeNTDLLFunctions()
{
	if (hNtDll)
		FreeLibrary(hNtDll);

	gNtQueryInformationProcess = NULL;
}

DWORD GetParentProcessID(DWORD dwProcessId)
{
	LONG     status;
	DWORD    dwParentPID = -3;
	HANDLE   hProcess;
	PROCESS_BASIC_INFORMATION    pbi;

	if (NULL == gNtQueryInformationProcess)
	{
		return -1;
	}


	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
	if (!hProcess)
	{
		return -2;
	}

	status = gNtQueryInformationProcess(hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL);


	if (!status)
	{
		dwParentPID = (ULONG_PTR)pbi.Reserved3;
	}
	else
	{
		WCHAR TT[MAX_PATH] = { 0 };
		wsprintf(TT, L"NtQueryInformationProcess Error: %x", status);

		OutputDebugString(TT);
	}

	CloseHandle(hProcess);

	return dwParentPID;

}

int GetProcessPath(DWORD idProcess, wchar_t* ParentProcessPath)
{
	wchar_t PProcessPath[MAX_PATH] = { 0 };

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, idProcess);
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			DWORD dw = GetModuleFileNameEx(hProcess, hMod, PProcessPath, MAX_PATH);
			lstrcpy(ParentProcessPath, PProcessPath);
		}
		CloseHandle(hProcess);
	}
	return 0;
}


/**
* 进程过滤
* =========================================================*/
bool processGuard(LPWSTR lpCommandLine)
{
	wchar_t username[64] = { 0 };
	DWORD dwSize = 64;
	GetUserName(username, &dwSize);

	/**
	* 涉危进程列表
	*/
	wchar_t *ProcessName[] =
	{
		L"control.exe",     //控制面板 
		L"mmc.exe",         //MMC
		L"notepad.exe",     //记事本
		L"CompMgmtLauncher.exe", // Win6.2 以上计算机管理
		L"ServerManager.exe",    // Win6.2 以上服务器管理 
		L"SystemSettings.exe",   // Win6.2 以上系统设置
		//L"explorer.exe",	 //资源管理器	
		L"msconfig.exe",     //系统配置
		L"msra.exe",         //远程协助 
		L"rstrui.exe",       //系统还原
		L"resmon.exe",       //资源监视器
		L"perfmon.exe",      //性能监视器  
		L"eventvwr.exe",     //事件查看器		
		L"regedit.exe",     //注册表编辑器
		L"taskmgr.exe",     //任务管理器
		L"winver.exe",      //关于
		L"msinfo32.exe",    //系统信息
		L"UserAccountControlSettings.exe", //UAC 设置
		L"powershell.exe",  // 
		L"cmd.exe"          // 
	};


	int size = sizeof(ProcessName) / sizeof(ProcessName[0]);

	bool findx = false;

	for (int i = 0; i < size; i++)
	{
		/**
		* 当前进程在涉危列表中...
		* StrStrIW 字符串包含（完整进程路径中是否包含有涉危进程表进行中的文件名）
		*/
		if (StrStrIW(lpCommandLine, ProcessName[i]) != NULL)
		{

			wstring userNameStr = username;
			wstring processStr = lpCommandLine;
			wstring outStr = userNameStr + L" Run:: " + processStr;

			OutputDebugString(outStr.c_str());

			/**
			* 非 Administrator 用户不允许启动涉危进程,返回 TRUE
			* wcsicmp 字符对比 （ 登录用户名对比  ） 忽略大小写
			**/
			if (wcsicmp(username, L"Administrator") != 0)
			{
				findx = true;
				break;
			}
		}
	}

	if (findx)
	{
		wchar_t buf[512 * 1] = { 0 };
		memset(buf, 0, sizeof(wchar_t) * 512 * 1);
		wsprintf(buf, L"Hook CreateProcess: %ls", lpCommandLine);
		OutputDebugStringW(buf);

		return true;
	}
	else
	{
		return false;
	}
}

/**
* 放行，特定的进程不 HOOK
*/
bool LefProcess()
{
	bool go = false;

	wchar_t modname[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, modname, sizeof(modname));

	wstring modulefilePath = modname;
	wstring moduleFileName = modulefilePath.substr(modulefilePath.rfind(L"\\") + 1);

	wchar_t *ProcessName[] =
	{
		L"rflow.exe",
		L"R.exe",
		L"Code.exe",
		L"winpty-agent.exe",  //vscode 调用 cmd、powershell 的客户端程序 
		L"Code - Insiders.exe"
	};

	int size = sizeof(ProcessName) / sizeof(ProcessName[0]);

	bool findx = false;
	for (int i = 0; i < size; i++)
	{
		/**
		* 忽略大小写，比较（字符串）文件名
		**/
		if (wcsicmp(moduleFileName.c_str(), ProcessName[i]) == 0)
		{
			wstring outText = L"Exit HOOK-DLL,RunFile: " + moduleFileName;
			OutputDebugString(outText.c_str());
			go = TRUE;
			break;
		}
	}
	return go;

}

//////////////////////////////////////////////////////////////////////////////////////////


int QuitProcess(DWORD processId, wchar_t* exitFunName)
{

	WCHAR outText[MAX_PATH * 2] = { 0 };

	wchar_t modname[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, modname, sizeof(modname));

	//DWORD processId = GetCurrentProcessId();
	DWORD sessionId = 0;
	ProcessIdToSessionId(processId, &sessionId);

	wchar_t sPath[MAX_PATH] = { 0 };

	int ParentPID = GetParentProcessID(processId);
	GetProcessPath(ParentPID, sPath);

	wsprintf(outText, L"HOOK %ls:: %ls processId:%d sessionId:%d ParentPID:%d ParentPPath:%ls", exitFunName, modname, processId, sessionId, ParentPID, sPath);
	OutputDebugString(outText);

	//C:\Windows\System32\rdpinit.exe
	/**
	* 父进程是否属于 rdpinit.exe ( RemoteAPP 应用 )
	**/
	if (wcsicmp(L"C:\\Windows\\System32\\rdpinit.exe", sPath) == 0)
	{
		//断开会话 + 注销会话
		//当 RemoteAPP 应用 结束时立即注销
		WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE, sessionId, FALSE);
		if (!WTSLogoffSession(WTS_CURRENT_SERVER_HANDLE, sessionId, FALSE))
		{
			OutputDebugString(L"WTSLogoffSession Error");
		}
	}

	return 0;
}