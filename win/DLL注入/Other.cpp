#include<Windows.h>
#include "tlhelp32.h"
#include<stdio.h>
#include <WtsApi32.h>
#include <UserEnv.h>
#include"StringHead.h"
#include"DLLUtils.h"
#define regsvr32Path L"C:\\Windows\\System32\\regsvr32.exe"

bool CreateDllUser(const stdstr& dllPath)
{
	HANDLE hToken = NULL;
	TCHAR szUsername[MAX_PATH];
	TCHAR para[MAX_PATH] = { 0 };
	TCHAR szUsernamePath[MAX_PATH];
	DWORD dwUsernameLen = MAX_PATH;
	DWORD cursessionid;
	cursessionid = WTSGetActiveConsoleSessionId();
	printf("WTSGetActiveConsoleSessionId() = %d\n", cursessionid);
	if (!WTSQueryUserToken(cursessionid, &hToken))
	{
		printf("%s WTSQueryUserToken is failed error:%i\n", __FUNCTION__, GetLastError());
	}
	//Copy
	HANDLE hDuplicatedToken = NULL;
	if (FALSE == DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hDuplicatedToken))
	{
		printf("%s DuplicateTokenEx is failed error:%i\n", __FUNCTION__, GetLastError());
	}
	//Env
	LPVOID environment;
	BOOL blockRet = CreateEnvironmentBlock(&environment, hDuplicatedToken, FALSE);
	if (!blockRet)
	{
		printf("could not create environment block (error: %i)",
			GetLastError());
	}

	BOOL bResult = FALSE;
	PROCESS_INFORMATION pi;
	PSID                psid;
	STARTUPINFO         si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	DWORD creationFlags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT;
	WCHAR command[1024] = { 0 };
	stdstr commandStr(regsvr32Path);
	commandStr += L" ";
	commandStr += dllPath;
	lstrcpy(command, commandStr.c_str());
	bResult = CreateProcessAsUser(
		hDuplicatedToken,
		NULL,
		command,
		NULL,
		NULL,
		FALSE,
		creationFlags,
		environment,
		NULL,
		&si,
		&pi
	);
	printf("CreateProcessAsUser0 = %d", GetLastError());
	if (bResult && pi.hProcess != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}
	else
	{
		printf("CreateProcessAsUser1 = %d", GetLastError());
	}
	if (pi.hThread != INVALID_HANDLE_VALUE)
		CloseHandle(pi.hThread);
	DestroyEnvironmentBlock(environment);

	RevertToSelf();
	CloseHandle(hToken);
	return true;
}

bool CreateDllSrc(const stdstr& dllPath)
{
	HANDLE hToken = NULL;
	TCHAR szUsername[MAX_PATH];
	TCHAR para[MAX_PATH] = { 0 };
	TCHAR szUsernamePath[MAX_PATH];
	DWORD dwUsernameLen = MAX_PATH;
	DWORD cursessionid;
	HANDLE hDuplicatedToken;
	do
	{
		cursessionid = WTSGetActiveConsoleSessionId();
		printf("WTSGetActiveConsoleSessionId() = %d\n", cursessionid);
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		{
			printf("%s WTSQueryUserToken is failed error:%i\n", __FUNCTION__, GetLastError());
			break;
		}
		//Copy
		hDuplicatedToken = NULL;
		if (FALSE == DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			printf("%s DuplicateTokenEx is failed error:%i\n", __FUNCTION__, GetLastError());
			break;
		}
		else
		{

		}
		if (FALSE == SetTokenInformation(hDuplicatedToken, _TOKEN_INFORMATION_CLASS::TokenSessionId, &cursessionid, sizeof(cursessionid)))
		{
			break;
		}
		STARTUPINFO	si;
		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		BOOL bResult = DLLUtils::CreateDLL(hDuplicatedToken, dllPath, si, pi);

		printf("CreateProcessAsUser0 = %d", GetLastError());
		if (bResult && pi.hProcess != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
		}
		else
		{
			printf("CreateProcessAsUser1 = %d", GetLastError());
		}
		if (pi.hThread != INVALID_HANDLE_VALUE)
			CloseHandle(pi.hThread);
	} while (0);
	RevertToSelf();
	CloseHandle(hDuplicatedToken);
	CloseHandle(hToken);
	return true;
}