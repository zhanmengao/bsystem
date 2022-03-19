#include"ForkUtils.h"
#include"../CreateProcess/ProcessHelper.h"
#pragma comment(lib, "UserEnv.lib")
bool ForkUtils::Exec(const char* path, UINT uiCmdShow)
{
	UINT uRet = WinExec(path, uiCmdShow);
	if (uRet > 31)
	{
		return true;
	}
	return false;
}
bool ForkUtils::Shell(const TCHAR* path, const TCHAR* parameter,
	const TCHAR* op, UINT uiCmdShow, HWND hParent, const TCHAR* workdir)
{
	return ShellExecute(hParent, op, path, parameter, workdir, uiCmdShow);
}
BOOL ForkUtils::ShellExec(const stdstr& cmdStr, const stdstr& Parameters, bool bWait)
{
	SHELLEXECUTEINFO  ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmdStr.c_str();
	ShExecInfo.lpParameters = Parameters.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	BOOL bRet = ShellExecuteEx(&ShExecInfo);
	if (bWait)
	{
		//aida运行采集数据，程序结束后，这里的阻塞才会向下执行
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);	
	}
	if (ShExecInfo.hProcess)
	{
		CloseHandle(ShExecInfo.hProcess);
		ShExecInfo.hProcess = NULL;
	}
	return bRet;
}
BOOL ForkUtils::OpenURL(const TCHAR* url)
{
	auto hinst = ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
	return hinst > 0;
}

BOOL ForkUtils::CreateEXE(const TCHAR* path, TCHAR* command, STARTUPINFO& si, PROCESS_INFORMATION& pi,
	DWORD dwFlag, BOOL bInherHandle, SECURITY_ATTRIBUTES* psa, SECURITY_ATTRIBUTES *tsa)
{
	return CreateProcess(path, command, psa, tsa, bInherHandle, dwFlag, NULL, NULL, &si, &pi);
}
BOOL ForkUtils::CreateEXE(const TCHAR* path, TCHAR* command, STARTUPINFOEX& six, PROCESS_INFORMATION& pi,
	DWORD dwFlag, BOOL bInherHandle, SECURITY_ATTRIBUTES* psa, SECURITY_ATTRIBUTES *tsa)
{
	return CreateProcess(path, command, psa, tsa, bInherHandle, dwFlag | EXTENDED_STARTUPINFO_PRESENT, 
		NULL, NULL, &six.StartupInfo, &pi);
}
DWORD ForkUtils::CreateEXE(const HANDLE& hToken, const TCHAR* exePath,
	STARTUPINFO& si, PROCESS_INFORMATION& pi, LogManager *log, DWORD priority)
{
	DWORD dwError = 0;
	HANDLE hDuplicatedToken = NULL;
	LPVOID lpEnvironment = NULL;
	do
	{
		// 复制令牌
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			dwError = GetLastError();
			if (log)
			{
				log->Log("Error DuplicateTokenEx " + to_string(dwError));
			}
			OutputDebugStringA(("Error DuplicateTokenEx " + to_string(dwError)).c_str());
			break;
		}

		// 创建用户Session环境
		if (FALSE == ::CreateEnvironmentBlock(&lpEnvironment, hDuplicatedToken, FALSE))
		{
			dwError = GetLastError();
			if (log)
			{
				log->Log("Error CreateEnvironmentBlock " + to_string(dwError));
			}
			OutputDebugStringA(("Error CreateEnvironmentBlock " + to_string(dwError)).c_str());
			break;
		}

		// 在复制的用户Session下执行应用程序，创建进程
		if (FALSE == ::CreateProcessAsUser(hDuplicatedToken,
			exePath, NULL, NULL, NULL, FALSE,
			priority | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
			lpEnvironment, NULL, &si, &pi))
		{
			dwError = GetLastError();
			if (log)
			{
				log->Log("Error CreateProcessAsUser Error " + to_string(dwError));
			}
			OutputDebugStringA(("Error CreateProcessAsUser " + to_string(dwError)).c_str());
			break;
		}
	} while (FALSE);
	// 关闭句柄, 释放资源
	if (lpEnvironment != NULL)
	{
		::DestroyEnvironmentBlock(lpEnvironment);
		lpEnvironment = NULL;
	}
	if (hDuplicatedToken != NULL)
	{
		::CloseHandle(hDuplicatedToken);
		hDuplicatedToken = NULL;
	}
	return dwError;
}
void ForkUtils::KillProcess(UINT exitCode)
{
	ExitProcess(exitCode);
}
void ForkUtils::KillProcess(const stdstr& processName)
{
	std::map<DWORD, ProcessInfo> mapProcess;
	ProcessHelper::GetProcessList(mapProcess);
	for (auto it = mapProcess.begin(); it != mapProcess.end(); it++)
	{
		if (it->second.szExeFile == processName)
		{
			KillProcess(it->first);
		}
	}
}
BOOL ForkUtils::KillProcess(DWORD nProcessID, UINT exitCode)
{
	HANDLE hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
	if (hProcessHandle != NULL)
	{
		BOOL bRet = ::TerminateProcess(hProcessHandle, exitCode);
		CloseHandle(hProcessHandle);
		return bRet;
	}
	return FALSE;
}
DWORD ForkUtils::Wait(HANDLE hProcess, bool bBlock)
{
	if (bBlock)
	{
		return WaitForSingleObject(hProcess, INFINITE);
	}
	else
	{
		return WaitForSingleObject(hProcess, 10);
	}
}
STARTUPINFO ForkUtils::GetStartInfo(bool bExtendParent)
{
	STARTUPINFO si = { 0 };
	if (bExtendParent)
	{
		 GetStartupInfo(&si);
	}
	else
	{
		si.cb = sizeof(si);
		si.lpReserved = NULL;
		si.lpReserved2 = NULL;
	}
	return si;

}
void ForkUtils::SetStartPos(STARTUPINFO &si, DWORD x, DWORD y, DWORD w, DWORD h)
{
	si.dwX = x;
	si.dwY = y;
	si.dwXSize = w;
	si.dwYSize = h;
	si.dwFlags |= STARTF_USESIZE;
	si.dwFlags |= STARTF_USEPOSITION;
}
void ForkUtils::SetStartConsoleTitle(STARTUPINFO &si, TCHAR* title)
{
	si.lpTitle = title;
}
void ForkUtils::SetStartWindowMax(STARTUPINFO &si, bool bMax)
{
	si.dwFlags |= STARTF_USESHOWWINDOW;
	if (bMax)
	{
		si.wShowWindow = SW_SHOWMAXIMIZED;
	}
	else
	{
		si.wShowWindow = SW_SHOWMINIMIZED;
	}
}
void ForkUtils::SetStartShowWindow(STARTUPINFO &si, bool bShow)
{
	si.dwFlags |= STARTF_USESHOWWINDOW;
	if (bShow)
	{
		si.wShowWindow = SW_SHOW;
	}
	else
	{
		si.wShowWindow = SW_HIDE;
	}
}
void ForkUtils::SetStartIO(STARTUPINFO &si, HANDLE input, HANDLE output, HANDLE nError)
{
	if (input != NULL)
	{
		si.hStdInput = input;
	}

	if (output != NULL)
	{
		si.hStdOutput = output;
	}
	if (nError != NULL)
	{
		si.hStdError = nError;
	}
	si.dwFlags |= STARTF_USESTDHANDLES;
}

STARTUPINFOEX ForkUtils::MallocStartInfoEX(bool bExtendParent)
{
	STARTUPINFOEX esi = { sizeof(STARTUPINFOEX) };
	esi.StartupInfo = GetStartInfo(bExtendParent);
	//为属性列表申请内存
	SIZE_T cbAttributeListSize = 0;
	InitializeProcThreadAttributeList(NULL, 1, 0, &cbAttributeListSize);
	esi.lpAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, cbAttributeListSize);
	InitializeProcThreadAttributeList(esi.lpAttributeList, 1, 0, &cbAttributeListSize);
	return esi;
}
void ForkUtils::FreeStartInfoEX(STARTUPINFOEX& esi)
{
	if (esi.lpAttributeList != NULL)
	{
		DeleteProcThreadAttributeList(esi.lpAttributeList);
		esi.lpAttributeList = NULL;
	}
}
BOOL ForkUtils::SetStartParent(STARTUPINFOEX &esi, HANDLE hParent)
{
	return UpdateProcThreadAttribute(esi.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
		&hParent, sizeof(HANDLE), NULL, NULL);
}
BOOL ForkUtils::SetStartHandleList(STARTUPINFOEX &esi, HANDLE *hHandle, size_t nCount)
{
	return UpdateProcThreadAttribute(esi.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
		hHandle, sizeof(HANDLE)*nCount, NULL, NULL);
}

//通过白名单程序提权启动程序
BOOL ForkUtils::UACWhiteListOpen(const TCHAR* path)
{
	//Software\\Classes 余下的每一项都要检测 并还原，该删除删除，原先有就保留
	HKEY hKey = NULL;
	::RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\mscfile\\shell\\open\\command"), 0, NULL, 0,
		KEY_WOW64_64KEY | KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if (hKey == NULL)
	{
		return FALSE;
	}

	//缓存老的值
	TCHAR cache[1024] = { 0 };
	DWORD dwType = 0;
	DWORD szCache = sizeof(cache);
	::RegQueryValueEx(hKey, NULL, NULL, &dwType, (BYTE*)cache, &szCache);

	//设置键值
	::RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)path, (1 + _tcslen(path)) * sizeof(TCHAR));

	//启动
	ShellExec(_T("C:\Windows\System32\CompMgmtLauncher.exe"), _T(""), true);
	::RegCloseKey(hKey);
}