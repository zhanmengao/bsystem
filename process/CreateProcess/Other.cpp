#include<Windows.h>
#include <WtsApi32.h>
#include "tlhelp32.h"
#include<string>
#include <UserEnv.h>
#include<tchar.h>
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;

static bool CreateExeByProcess(const wstring& exePath)
{
	HANDLE hToken = NULL;
	TCHAR para[MAX_PATH] = { 0 };
	DWORD dwUsernameLen = MAX_PATH;
	DWORD cursessionid = WTSGetActiveConsoleSessionId();
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		return false;
	}
	HANDLE hDuplicatedToken = NULL;
	// 复制令牌
	if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
		SecurityIdentification, TokenPrimary, &hDuplicatedToken))
	{
		return false;
	}
	//
	if (!SetTokenInformation(hDuplicatedToken, TokenSessionId, &cursessionid, sizeof(cursessionid)))
	{
		return false;
	}
	BOOL bResult = FALSE;
	PROCESS_INFORMATION pi;
	STARTUPINFO         si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	LPVOID environment;
	BOOL blockRet = CreateEnvironmentBlock(&environment, hDuplicatedToken, FALSE);
	if (!blockRet)
	{
		return false;
	}
	else
	{
		DWORD creationFlags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT;
		bResult = CreateProcessAsUser(
			hDuplicatedToken,
			(exePath).c_str(),
			NULL,
			NULL,
			NULL,
			FALSE,
			creationFlags,
			environment,
			NULL,
			&si,
			&pi);
		if (!bResult)
		{
			return false;
		}
		if (bResult && pi.hProcess != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
		}
		if (pi.hThread != INVALID_HANDLE_VALUE)
			CloseHandle(pi.hThread);
	}
	DestroyEnvironmentBlock(environment);
	CloseHandle(hToken);
	return true;
}
static bool CreateDllByProcess(const wstring& dllPath)
{
	HANDLE hToken = NULL;
	TCHAR para[MAX_PATH] = { 0 };
	DWORD cursessionid = WTSGetActiveConsoleSessionId();
	//获取当前进程的用户令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		return false;
	}
	HANDLE hDuplicatedToken = NULL;
	// 复制令牌
	if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
		SecurityIdentification, TokenPrimary, &hDuplicatedToken))
	{
		return false;
	}
	//建立当前进程权限，但是会话在当前用户的 高权限token
	if (!SetTokenInformation(hDuplicatedToken, TokenSessionId, &cursessionid, sizeof(cursessionid)))
	{
		return false;
	}
	BOOL bResult = FALSE;
	PROCESS_INFORMATION pi;
	STARTUPINFO         si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	LPVOID environment;
	BOOL blockRet = CreateEnvironmentBlock(&environment, hDuplicatedToken, FALSE);
	if (!blockRet)
	{
		return false;
	}
	else
	{
		DWORD creationFlags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT;
		wstring commandStr = PathHelper::GetSysFolder() + L"regsvr32.exe " + dllPath;
		TCHAR command[1024] = { 0 };
		lstrcpynW(command, commandStr.c_str(), 1024);
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
			&pi);
		if (!bResult)
		{
			return false;
		}
		if (bResult && pi.hProcess != INVALID_HANDLE_VALUE)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
		}
		if (pi.hThread != INVALID_HANDLE_VALUE)
			CloseHandle(pi.hThread);
	}
	DestroyEnvironmentBlock(environment);
	CloseHandle(hToken);
	return true;
}
BOOL TriggerAppExecute(std::wstring wstrCmdLine/*, INT32& n32ExitResult*/)
{
	DWORD dwProcesses = 0;
	BOOL bResult = FALSE;

	DWORD dwSid = WTSGetActiveConsoleSessionId();

	DWORD dwRet = 0;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	HANDLE hProcess = NULL, hPToken = NULL, hUserTokenDup = NULL;
	if (!WTSQueryUserToken(dwSid, &hPToken))
	{
		PROCESSENTRY32 procEntry;
		DWORD dwPid = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		procEntry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (_tcsicmp(procEntry.szExeFile, _T("explorer.exe")) == 0)
				{
					DWORD exeSessionId = 0;
					if (ProcessIdToSessionId(procEntry.th32ProcessID, &exeSessionId) && exeSessionId == dwSid)
					{
						dwPid = procEntry.th32ProcessID;
						break;
					}
				}

			} while (Process32Next(hSnap, &procEntry));
		}
		CloseHandle(hSnap);

		// explorer进程不存在
		if (dwPid == 0)
		{
			return FALSE;
		}

		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
		if (hProcess == NULL)
		{
			return FALSE;
		}

		if (!::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS_P, &hPToken))
		{
			CloseHandle(hProcess);
			return FALSE;
		}
	}

	if (hPToken == NULL)
		return FALSE;

	TOKEN_LINKED_TOKEN admin;
	bResult = GetTokenInformation(hPToken, (TOKEN_INFORMATION_CLASS)TokenLinkedToken, &admin, sizeof(TOKEN_LINKED_TOKEN), &dwRet);

	if (!bResult) // vista 以前版本不支持TokenLinkedToken
	{
		TOKEN_PRIVILEGES tp;
		LUID luid;
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
		//复制token
		DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);
	}
	else
	{
		hUserTokenDup = admin.LinkedToken;
	}

	LPVOID pEnv = NULL;
	DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;

	// hUserTokenDup为当前登陆用户的令牌
	if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		//如果传递了环境变量参数，CreateProcessAsUser的
		//dwCreationFlags参数需要加上CREATE_UNICODE_ENVIRONMENT，
		//这是MSDN明确说明的
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
	{
		//环境变量创建失败仍然可以创建进程，
		//但会影响到后面的进程获取环境变量内容
		pEnv = NULL;
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	si.hStdError = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	ZeroMemory(&pi, sizeof(pi));
	WCHAR cmdPath[1024];
	GetSystemDirectory(cmdPath, sizeof(cmdPath));
	lstrcat(cmdPath, L"\\cmd.exe");
	bResult = CreateProcessAsUser(
		hUserTokenDup,                     // client's access token
		cmdPath,    // file to execute
		(LPTSTR)wstrCmdLine.c_str(),                 // command line
		NULL,            // pointer to process SECURITY_ATTRIBUTES
		NULL,               // pointer to thread SECURITY_ATTRIBUTES
		FALSE,              // handles are not inheritable
		dwCreationFlags,     // creation flags
		pEnv,               // pointer to new environment block
		NULL,               // name of current directory
		&si,               // pointer to STARTUPINFO structure
		&pi                // receives information about new process
	);
	if (!bResult)
	{
		printf("CreateProcessAsUser Error: %i \n", GetLastError());
	}

	if (pi.hProcess != INVALID_HANDLE_VALUE && bResult)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	if (hUserTokenDup != NULL)
		CloseHandle(hUserTokenDup);
	if (hProcess != NULL)
		CloseHandle(hProcess);
	if (hPToken != NULL)
		CloseHandle(hPToken);
	if (pEnv != NULL)
		DestroyEnvironmentBlock(pEnv);

	return TRUE;
}