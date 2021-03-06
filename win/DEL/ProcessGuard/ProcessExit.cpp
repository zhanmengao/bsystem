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
* ????????
* =========================================================*/
bool processGuard(LPWSTR lpCommandLine)
{
	wchar_t username[64] = { 0 };
	DWORD dwSize = 64;
	GetUserName(username, &dwSize);

	/**
	* ????????????
	*/
	wchar_t *ProcessName[] =
	{
		L"control.exe",     //???????? 
		L"mmc.exe",         //MMC
		L"notepad.exe",     //??????
		L"CompMgmtLauncher.exe", // Win6.2 ??????????????
		L"ServerManager.exe",    // Win6.2 ?????????????? 
		L"SystemSettings.exe",   // Win6.2 ????????????
		//L"explorer.exe",	 //??????????	
		L"msconfig.exe",     //????????
		L"msra.exe",         //???????? 
		L"rstrui.exe",       //????????
		L"resmon.exe",       //??????????
		L"perfmon.exe",      //??????????  
		L"eventvwr.exe",     //??????????		
		L"regedit.exe",     //????????????
		L"taskmgr.exe",     //??????????
		L"winver.exe",      //????
		L"msinfo32.exe",    //????????
		L"UserAccountControlSettings.exe", //UAC ????
		L"powershell.exe",  // 
		L"cmd.exe"          // 
	};


	int size = sizeof(ProcessName) / sizeof(ProcessName[0]);

	bool findx = false;

	for (int i = 0; i < size; i++)
	{
		/**
		* ????????????????????...
		* StrStrIW ??????????????????????????????????????????????????????????????
		*/
		if (StrStrIW(lpCommandLine, ProcessName[i]) != NULL)
		{

			wstring userNameStr = username;
			wstring processStr = lpCommandLine;
			wstring outStr = userNameStr + L" Run:: " + processStr;

			OutputDebugString(outStr.c_str());

			/**
			* ?? Administrator ??????????????????????,???? TRUE
			* wcsicmp ???????? ?? ??????????????  ?? ??????????
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
* ?????????????????? HOOK
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
		L"winpty-agent.exe",  //vscode ???? cmd??powershell ???????????? 
		L"Code - Insiders.exe"
	};

	int size = sizeof(ProcessName) / sizeof(ProcessName[0]);

	bool findx = false;
	for (int i = 0; i < size; i++)
	{
		/**
		* ????????????????????????????????
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
	* ?????????????? rdpinit.exe ( RemoteAPP ???? )
	**/
	if (wcsicmp(L"C:\\Windows\\System32\\rdpinit.exe", sPath) == 0)
	{
		//???????? + ????????
		//?? RemoteAPP ???? ??????????????
		WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE, sessionId, FALSE);
		if (!WTSLogoffSession(WTS_CURRENT_SERVER_HANDLE, sessionId, FALSE))
		{
			OutputDebugString(L"WTSLogoffSession Error");
		}
	}

	return 0;
}