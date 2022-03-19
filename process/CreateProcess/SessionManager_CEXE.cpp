#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"../Fork/ForkUtils.h"
#include"SessionManager.h"
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include"../../WSessionManager/SessionManager/SessionFork.h"
std::vector<BOOL> SessionForkUtils::CreateEXE_ALL_ME(const stdstr& exePath)
{
	std::vector<BOOL> ret;
	std::vector<DWORD>sessionList = GetActiveSessionList();
	ret.resize(sessionList.size());
	for (auto it = sessionList.begin(); it != sessionList.end(); it++)
	{
		BOOL bRet = CreateEXE_Me(exePath, *it);
		ret.push_back(bRet);
	}
	return ret;
}
std::vector<BOOL> SessionForkUtils::CreateEXE_ALL_Session(const stdstr& exePath)
{
	std::vector<BOOL> ret;
	std::vector<DWORD>sessionList = SessionManager::GetActiveSessionList();
	ret.resize(sessionList.size());
	for (auto it = sessionList.begin(); it != sessionList.end(); it++)
	{
		// 获得当前Session ID
		BOOL bRet = CreateEXE_Session(exePath, *it);
		ret.push_back(bRet);
	}
	return ret;
}
DWORD SessionForkUtils::CreateEXE_Session(const stdstr& exePath, DWORD session, bool bGuard, LogManager* log)
{
	DWORD dwRet = 0;
	do
	{
		if (log)
		{
#ifdef _UNICODE
			log->LogDaily("Try create process " + Convert::WStringToString(exePath) + " to " + to_string(session));
#else
			log->LogDaily("Try create process " + exePath + "to " + to_string(session));
#endif
		}
		HANDLE hToken = NULL;
		STARTUPINFO si = { 0 };
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi = { 0 };
		do
		{
			// 获得当前Session的用户令牌
			hToken = GetToken(session);
			if (hToken == NULL)
			{
				dwRet = GetLastError();
				if (log)
				{
					log->Log("GetToken(session) Error " + to_string(GetLastError()));
				}
				break;
			}
			dwRet = ForkUtils::CreateEXE(hToken, exePath.c_str(), si, pi,log);
		} while (false);
		if (log)
		{
			log->LogDaily("Create EXE to session" + to_string(session) + " ret :  " + to_string(dwRet));
		}
		if (hToken != NULL)
		{
			CloseHandle(hToken);
			hToken = NULL;
		}
		if (pi.hThread != NULL)
		{
			CloseHandle(pi.hThread);
			pi.hThread = NULL;
		}
		//如果需要守护
		DWORD dwExit  = 0;
		if (bGuard && pi.hProcess != NULL)
		{		
			WaitForSingleObject(pi.hProcess, INFINITE);
			GetExitCodeProcess(pi.hProcess, &dwExit);
			log->Log("WaitForSingleObject Return . Exit Code :" + to_string(dwExit));
		}
		if (pi.hProcess != NULL)
		{
			CloseHandle(pi.hProcess);
			pi.hProcess = NULL;
		}	
		Sleep(5000);
	} while (bGuard);

	return dwRet;
}
DWORD SessionForkUtils::CreateEXE_Me(const stdstr& exePath, DWORD session, bool bGuard, LogManager* log)
{
	DWORD bRet;
	do
	{
		HANDLE hToken = NULL;
		HANDLE hDuplicatedToken = NULL;
		STARTUPINFO si = { 0 };
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi = { 0 };
		do
		{
			//打开当前进程的token
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
			{
				bRet = GetLastError();
				if (log)
				{
					log->Log(("Error OpenProcessToken " + to_string(GetLastError())));
				}
				OutputDebugStringA(("Error OpenProcessToken " + to_string(GetLastError())).c_str());
				break;
			}
			// 复制token
			if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
				SecurityIdentification, TokenPrimary, &hDuplicatedToken))
			{
				bRet = GetLastError();
				if (log)
				{
					log->Log(("Error DuplicateTokenEx " + to_string(GetLastError())));
				}
				OutputDebugStringA(("Error DuplicateTokenEx " + to_string(GetLastError())).c_str());
				break;
			}
			//将复制的token的session改为用户session
			if (!SetTokenInformation(hDuplicatedToken, TokenSessionId, &session, sizeof(session)))
			{
				bRet = GetLastError();
				if (log)
				{
					log->Log(("Error SetTokenInformation " + to_string(GetLastError())));
				}
				OutputDebugStringA(("Error SetTokenInformation " + to_string(GetLastError())).c_str());
				break;
			}
			bRet = ForkUtils::CreateEXE(hDuplicatedToken, exePath.c_str(), si, pi,log);
		} while (false);
		if (hDuplicatedToken != NULL)
		{
			CloseHandle(hDuplicatedToken);
			hDuplicatedToken = NULL;
		}
		if (hToken != NULL)
		{
			CloseHandle(hToken);
			hToken = NULL;
		}
		if (pi.hThread != NULL)
		{
			CloseHandle(pi.hThread);
			pi.hThread = NULL;
		}
		//如果需要守护
		DWORD dwExit = 0;
		if (bGuard && pi.hProcess != NULL)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			GetExitCodeProcess(pi.hProcess, &dwExit);
			log->Log("WaitForSingleObject Return,Exit Code :" + to_string(dwExit));
		}
		if (pi.hProcess != NULL)
		{
			CloseHandle(pi.hProcess);
			pi.hProcess = NULL;
		}
		Sleep(1000);
	} while (bGuard);

	return bRet;
}

BOOL SessionForkUtils::WTSKillProcess(DWORD pid)
{
	return WTSTerminateProcess(WTS_CURRENT_SERVER_HANDLE, pid, 0);
}
