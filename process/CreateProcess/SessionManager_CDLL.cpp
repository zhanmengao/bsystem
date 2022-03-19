#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"SessionManager.h"
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include"../../../Windows/DLLProject/DLL注入/DLLUtils.h"
#include"../../WSessionManager/SessionManager/SessionFork.h"
std::vector<BOOL> SessionForkUtils::CreateDLL_ALL_ME(const stdstr& dllPath)
{
	std::vector<BOOL> ret;
	std::vector<DWORD>sessionList = GetActiveSessionList();
	ret.resize(sessionList.size());
	for (auto it = sessionList.begin(); it != sessionList.end(); it++)
	{
		BOOL bRet = CreateDLL_Me(dllPath, *it);
		ret.push_back(bRet);
	}
	return ret;
}
std::vector<BOOL> SessionForkUtils::CreateDLL_ALL_Session(const stdstr& dllPath)
{
	std::vector<BOOL> ret;
	std::vector<DWORD>sessionList = GetActiveSessionList();
	ret.resize(sessionList.size());
	for (auto it = sessionList.begin(); it != sessionList.end(); it++)
	{
		BOOL bRet = CreateDLL_Session(dllPath, *it);
		ret.push_back(bRet);
	}
	return ret;
}
BOOL SessionForkUtils::CreateDLL_Session(const stdstr& exePath, DWORD session)
{
	BOOL bRet = TRUE;
	// 获得当前Session的用户令牌
	HANDLE hToken = GetToken(session);
	if (hToken == NULL)
	{
		bRet = FALSE;
		return bRet;
	}
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	bRet = DLLUtils::CreateDLL(hToken, exePath.c_str(), si, pi);
	if (hToken != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hToken);
		hToken = INVALID_HANDLE_VALUE;
	}
	return bRet;
}
BOOL SessionForkUtils::CreateDLL_Me(const stdstr& exePath, DWORD session)
{
	BOOL bRet;
	HANDLE hToken = INVALID_HANDLE_VALUE;
	HANDLE hDuplicatedToken = INVALID_HANDLE_VALUE;
	do
	{
		//打开当前进程的token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		{
			bRet = FALSE;
			break;
		}
		// 复制token
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			bRet = false;
			break;
		}
		//将复制的token的session改为用户session
		if (!SetTokenInformation(hDuplicatedToken, TokenSessionId, &session, sizeof(session)))
		{
			bRet = false;
			break;
		}
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		bRet = DLLUtils::CreateDLL(hDuplicatedToken, exePath, si, pi);
	} while (false);
	if (hDuplicatedToken != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hDuplicatedToken);
		hDuplicatedToken = INVALID_HANDLE_VALUE;
	}
	if (hToken != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hToken);
		hToken = INVALID_HANDLE_VALUE;
	}
	return bRet;
}