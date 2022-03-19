#include"SessionManager.h"
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include<stdio.h>
#include<string.h>
#include"ProcessHelper.h"
RDPLoginUserInfo SessionManager::GetLoginUserInfo(DWORD SessionId)
{
	WTS_INFO_CLASS WTSInfoClass;
	DWORD pCount;
	TCHAR* ppBuffer;

	RDPLoginUserInfo rinfo;
	rinfo.SClear();

	WTSInfoClass = WTSInitialProgram;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSInitialProgram = ppBuffer;

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSApplicationName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSApplicationName = ppBuffer;

		WTSFreeMemory(ppBuffer);

	}


	WTSInfoClass = WTSWorkingDirectory;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSWorkingDirectory = ppBuffer;

		WTSFreeMemory(ppBuffer);

	}


	WTSInfoClass = WTSUserName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSUserName = ppBuffer;

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSWinStationName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSWinStationName = ppBuffer;

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSDomainName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSDomainName = ppBuffer;
		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSClientName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSClientName = ppBuffer;
		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSClientDirectory;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		rinfo.WTSClientDirectory = ppBuffer;

		WTSFreeMemory(ppBuffer);
	}



	WTSInfoClass = WTSClientAddress;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		PWTS_CLIENT_ADDRESS pWTSCA = (PWTS_CLIENT_ADDRESS)ppBuffer;

		if (pWTSCA->AddressFamily != 0)
		{
			TCHAR buf[MAX_PATH] = { 0 };
			wsprintf(buf, _T("%d.%d.%d.%d"),
				pWTSCA->Address[2],
				pWTSCA->Address[3],
				pWTSCA->Address[4],
				pWTSCA->Address[5]);
			rinfo.WTSClientAddress = buf;
		}

		WTSFreeMemory(ppBuffer);

	}



	WTSInfoClass = WTSClientInfo;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		PWTSCLIENT test = (PWTSCLIENT)ppBuffer;

		rinfo.ClientBuildNumber = test->ClientBuildNumber;
		rinfo.ClientHardwareId = test->ClientHardwareId;
		rinfo.ClientProductId = test->ClientProductId;
		rinfo.ColorDepth = test->ColorDepth;
		rinfo.HRes = test->HRes;
		rinfo.VRes = test->VRes;
		rinfo.EncryptionLevel = test->EncryptionLevel;

		rinfo.DeviceId = test->DeviceId;

		WTSFreeMemory(ppBuffer);

	}


	WTSInfoClass = WTSSessionInfo;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		PWTSINFO test = (PWTSINFO)ppBuffer;
		rinfo.SessionId = test->SessionId;
		WTSFreeMemory(ppBuffer);
	}


	return rinfo;

}

HANDLE SessionManager::GetToken(DWORD session)
{
	HANDLE hToken = NULL;
	HANDLE hProcess = NULL;
	HANDLE hSnap = NULL;
	do
	{
		if (!WTSQueryUserToken(session, &hToken))
		{
			PROCESSENTRY32 procEntry;
			DWORD dwPid = 0;
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hSnap == NULL)
			{
				break;
			}
			procEntry.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hSnap, &procEntry))
			{
				do
				{
					if (_tcsicmp(procEntry.szExeFile, _T("explorer.exe")) == 0)
					{
						DWORD exeSessionId = 0;
						if (ProcessIdToSessionId(procEntry.th32ProcessID, &exeSessionId)
							&& exeSessionId == session)
						{
							dwPid = procEntry.th32ProcessID;
							break;
						}
					}
				} while (Process32Next(hSnap, &procEntry));
			}
			// explorer进程不存在
			if (dwPid == 0)
			{
				::SetLastError(127);
				break;
			}
			hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
			if (hProcess == NULL)
			{
				break;
			}

			if (!::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS_P, &hToken))
			{
				break;
			}
		}
	} while (false);
	if (hProcess != NULL)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}
	if (hSnap != NULL)
	{
		CloseHandle(hSnap);
		hSnap = NULL;
	}
	return hToken;
}
BOOL SessionManager::GetWTSProcesses(DWORD SessionId, map<DWORD, WTSProcessInfo> &promap)
{
	promap.clear();
	DWORD Level = 1;
	WTS_PROCESS_INFO_EX* ppProcessInfo = NULL;
	DWORD pCount = 0;
	DWORD rdpinit_ProcessId = 0;
	BOOL bRet = WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &Level, SessionId, (TCHAR**)&ppProcessInfo, &pCount);
	if (bRet)
	{
		for (unsigned i = 0; i < pCount; i++)
		{
			auto& process = ppProcessInfo[i];
			promap.insert(std::make_pair(process.ProcessId, process));
		}
		WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, ppProcessInfo, pCount);
	}
	return bRet;
}
BOOL SessionManager::GetRDPinit(DWORD SessionId, WTSProcessInfo &rdpProcess, const TCHAR* rdpinit)
{
	map<DWORD, WTSProcessInfo> promap;
	BOOL bRet = GetWTSProcesses(SessionId, promap);
	if (!bRet)
	{
		return bRet;
	}
	bRet = FALSE;
	for (auto it = promap.begin(); it != promap.end(); it++)
	{
		/***
		*  rdpinit.exe 进程属于 RemoteAPP 初始化程，有这个进程存在，则表明当前会话可能是由RemoteAPP 发起的；
		*  通过查找该进程的子进程，即可以查询出 RemoteAPP 应用程序
		*/
		if (it->second.pProcessName.find(rdpinit) != std::string::npos)
		{
			rdpProcess = it->second;
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}
BOOL SessionManager::GetRemoteAPPBySID(DWORD SessionId, map<DWORD, ProcessInfo> &promap)
{
	promap.clear();
	WTSProcessInfo remoteApp;
	BOOL bRet = GetRDPinit(SessionId, remoteApp);
	if (!bRet)
	{
		return bRet;
	}
	return GetRemoteAPPByPID(remoteApp.ProcessId, promap);
}
BOOL SessionManager::GetRemoteAPPByPID(DWORD ProcessID, map<DWORD, ProcessInfo> &promap)
{
	std::map<DWORD, ProcessInfo> _mapProcess;
	ProcessHelper::GetProcessList(_mapProcess);
	return GetRemoteAPPByPID(_mapProcess, ProcessID, promap);
}
BOOL SessionManager::GetRemoteAPPByPID(const std::map<DWORD, ProcessInfo>& _mapProcess,
	DWORD ProcessID, map<DWORD, ProcessInfo> &promap)
{
	vector<ProcessInfo> vProcess;
	ProcessHelper::GetProcessfromParentID(_mapProcess, ProcessID, vProcess);
	for (auto it = vProcess.begin(); it != vProcess.end(); it++)
	{
		ProcessInfo data;
		data = *it;
		//如果是OpenHomeFile，则找他的子进程
		if (it->szExeFile.find(_T("OpenHomeFile")) != std::string::npos)
		{
			//递归找到合适的子进程
			GetRemoteAPPByPID(_mapProcess, it->th32ProcessID, promap);
			continue;
		}
		/** 忽略掉 rdpshell.exe 它是RemoteAPP 的外壳程序 **/
		else if (lstrcmp(_T("rdpshell.exe"), it->szExeFile.c_str()) == 0)
		{
			continue;
		}
		else if (lstrcmp(_T("conhost.exe"), it->szExeFile.c_str()) == 0)
		{
			continue;
		}
		//0 和 4不视为虚拟应用
		else if (it->th32ProcessID == 0 || it->th32ProcessID == 4)
		{
			continue;
		}
		promap.insert(std::make_pair(data.th32ProcessID, data));
	}
	return promap.size() >0;
}
stdstr SessionManager::GetRemoteAPP(DWORD SessionId)
{
	stdstr name;
	TCHAR* ppBuffer;
	DWORD pCount;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInitialProgram,
		&ppBuffer,
		&pCount))
	{
		TCHAR InitialProgram[128] = { 0 };
		lstrcpy(InitialProgram, ppBuffer);
		name = InitialProgram;
		WTSFreeMemory(ppBuffer);
	}
	return name;
}
DWORD SessionManager::GetSessionID(const TCHAR* user)
{
	std::vector<RDPLoginSessionInfo> vec;
	if (GetWTSSessions(vec))
	{
		return ULONG_MAX;
	}
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if (lstrcmp(it->User.WTSUserName.c_str(), user) == 0)
		{
			return it->sid;
		}
	}
	return ULONG_MAX;
}
DWORD SessionManager::GetSessionID()
{
	DWORD pid = GetCurrentProcessId();
	return GetSessionID(pid);
}
DWORD SessionManager::GetSessionID(DWORD pid)
{
	DWORD sid;
	ProcessIdToSessionId(pid, &sid);
	return sid;
}
DWORD SessionManager::GetActiveSessionID()
{
	DWORD dwSessionId = 0;
	PWTS_SESSION_INFO pSessionInfo = NULL;
	DWORD dwCount = 0;

	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);

	for (DWORD i = 0; i < dwCount; i++)
	{
		WTS_SESSION_INFO si = pSessionInfo[i];
		if (WTSActive == si.State)
		{
			dwSessionId = si.SessionId;
			break;
		}
	}
	WTSFreeMemory(pSessionInfo);
	return dwSessionId;
}