#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"SessionManager.h"
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include"../../../Windows/DLLProject/DLLע��/DLLUtils.h"
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
	// ��õ�ǰSession���û�����
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
		//�򿪵�ǰ���̵�token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		{
			bRet = FALSE;
			break;
		}
		// ����token
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			bRet = false;
			break;
		}
		//�����Ƶ�token��session��Ϊ�û�session
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