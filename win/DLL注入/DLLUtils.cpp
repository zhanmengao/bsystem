#include<Windows.h>
#include"DLLUtils.h"
#include <WtsApi32.h>
#include "tlhelp32.h"
#include <UserEnv.h>
#include"ProcessHelper.h"

#include"WinStrUtils.h"
#include"PathHelper.hpp"

#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "Wtsapi32.lib")
void DLLUtils::InjectDLLThread(DWORD pid, const char* dllPath)
{
	if (pid == NULL || strlen(dllPath) == 0)
	{
		return;
	}
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	do
	{
		//打开目标进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}

		//在目标进程申请内存
		unsigned dllLen = strlen(dllPath) + sizeof(char);
		PVOID pDllAddr = VirtualAllocEx(hProcess, NULL, dllLen, MEM_COMMIT, PAGE_READWRITE);
		if (pDllAddr == nullptr)
		{
			break;
		}

		//向目标进程内存写值
		SIZE_T dwWriteNum = 0;
		WriteProcessMemory(hProcess, pDllAddr, dllPath, dllLen, &dwWriteNum);
		
		//拿到对端函数入口地址（kernel32在任何机器地址都一致）
		FARPROC pFuncAddr = GetProcAddress(GetModuleHandleA(KERNEL), LOAD);
		if (pFuncAddr == nullptr)
		{
			break;
		}
		//远程创建线程
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)pFuncAddr, pDllAddr, NULL, NULL);
		if (hThread > 0)
		{
			printf("CreateRemoteThread OK \n");
			WaitForSingleObject(hThread, INFINITE);
		}
	} while (0);
	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		hProcess = INVALID_HANDLE_VALUE;
	}
}


void DLLUtils::InjectDLLThread(DWORD pid, RemoteThreadProc proc)
{
	if (pid == 0)
	{
		return;
	}
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	do
	{
		//打开目标进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}

		//地址相同，可在注入前获取
		_InjectDATA data;
		data.dwLoadLibrary = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), LOAD);
		data.dwGetProcAddress = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), "GetProcAddress");
		data.dwGetModuleHandle = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), "GetModuleHandleA");
		data.dwGetModuleFileName = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), "GetModuleFileNameA");
		strcpy_s(data.User32Dll,_countof(data.User32Dll),"user32.dll");
		strcpy_s(data.MsgBox, _countof(data.MsgBox), "MessageBoxA");
		strcpy_s(data.text, _countof(data.text), "Inject Code!!!");
		//在目标进程申请内存，写入结构体
		PVOID lpData = VirtualAllocEx(hProcess, NULL, sizeof(data), MEM_COMMIT, PAGE_READWRITE);
		if (lpData == nullptr)
		{
			break;
		}
		SIZE_T dwWriteNum = 0;
		WriteProcessMemory(hProcess, lpData, &data, sizeof(data), &dwWriteNum);

		//在目标进程申请内存，写入代码段
		DWORD dwFunSize = 0x4000;
		PVOID lpCode = VirtualAllocEx(hProcess, NULL, dwFunSize, MEM_COMMIT, PAGE_READWRITE);
		if (lpCode == nullptr)
		{
			break;
		}
		//向目标进程内存写值
		WriteProcessMemory(hProcess, lpCode, &proc, dwFunSize, &dwWriteNum);

		//远程创建线程
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)lpCode, lpData, NULL, NULL);
		WaitForSingleObject(hThread, INFINITE);
	} while (0);
	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		hProcess = INVALID_HANDLE_VALUE;
	}
}
void DLLUtils::InjectDLLAPC(DWORD pid, const char* dllPath)							//将DLL注入指定进程
{
	if (pid == 0 || strlen(dllPath) == 0)
	{
		return;
	}
	//计算dll文件路径长度
	int nDllLen = strlen(dllPath) + sizeof(char);

	//打开目标进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if (hProcess == NULL)
	{
		return;
	}
	//在目标进程申请内存
	PVOID pDllAddr = Malloc(hProcess, nDllLen);
	if (pDllAddr == NULL)
	{
		CloseHandle(hProcess);
		return;
	}
	DWORD dwWriteNum = 0;
	//写入内存
	WriteMemory(hProcess, pDllAddr, (void*)dllPath, nDllLen);
	CloseHandle(hProcess);

	//得到线程快照
	THREADENTRY32 te32 = { 0 };
	te32.dwSize = sizeof(te32);
	HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == handleSnap)
	{
		CloseHandle(hProcess);
		return;
	}
	//获取Load函数入口地址
	FARPROC pFuncAddr = GetProcAddress(GetModule(_T(KERNEL)), LOAD);

	//得到第一个线程
	DWORD dwRet = 0;
	if (Thread32First(handleSnap, &te32))
	{
		do
		{
			//进程id对比
			if (te32.th32OwnerProcessID == pid)
			{
				//得到线程句柄
				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
				if (hThread != NULL)
				{
					dwRet = QueueUserAPC((PAPCFUNC)pFuncAddr, hThread, (ULONG_PTR)pDllAddr);
					CloseHandle(hThread);
				}
			}
		} while (Thread32Next(handleSnap, &te32));
	}
	CloseHandle(handleSnap);
}
void DLLUtils::FreeAllDLL(const TCHAR* inDllPath)
{
	//遍历所有进程，清理其dll
	std::map<DWORD, ProcessInfo> mapProcess;
	if (ProcessHelper::GetProcessList(mapProcess))
	{
		for (auto it = mapProcess.begin(); it != mapProcess.end(); )
		{
			DWORD dwRet = FreeDLL(it->first, inDllPath);
			if (dwRet == 0 || dwRet == 5)
			{
				it++;
			}
			else
			{
				Sleep(2000);
			}
		}
	}
}
DWORD DLLUtils::FreeDLL(DWORD pid, const TCHAR* inDllPath)
{
	if (pid == NULL || lstrlen(inDllPath) == 0)
	{
		return 0;
	}
	HANDLE hSnap = INVALID_HANDLE_VALUE;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	TCHAR dllPath[MAX_PATH] = { 0 };
	lstrcpy(dllPath, inDllPath);
	TCHAR szExePath[MAX_PATH] = { 0 };
	do
	{
		//查找dll模块
		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		if (hSnap == INVALID_HANDLE_VALUE)
		{
			printf("CreateToolhelp32Snapshot pid = %ld Error %d \n", pid, GetLastError());
			break;
		}
		MODULEENTRY32 me32 = { 0 };
		me32.dwSize = sizeof(me32);
		BOOL bRet = Module32First(hSnap, &me32);
		while (bRet)
		{
			memset(szExePath, 0, sizeof(szExePath));
			lstrcpy(szExePath, me32.szExePath);
			if (lstrcmp(szExePath, dllPath) == 0)
			{
				bRet = TRUE;
				break;
			}
			else
			{
				bRet = Module32Next(hSnap, &me32);
			}
		}
		if (lstrcmp(szExePath, dllPath) != 0)
		{
			break;
		}
		//打开目标进程
		hProcess = OpenProcess(PROCESS_CREATE_THREAD, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}
		//拿到对端函数入口地址（kernel32在任何机器地址都一致）
		FARPROC pFuncAddr = GetProcAddress(GetModuleHandleA(KERNEL), FREE);
		if (pFuncAddr == nullptr)
		{
			break;
		}
		//远程创建线程调用free
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)pFuncAddr, me32.hModule, NULL, NULL);
		WaitForSingleObject(hThread, INFINITE);
		printf("%ld CreateRemoteThread Sec \n", pid);
	} while (0);
	if (hThread != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hThread);
		hThread = INVALID_HANDLE_VALUE;
	}
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		hProcess = INVALID_HANDLE_VALUE;
	}
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSnap);
		hSnap = INVALID_HANDLE_VALUE;
	}
	return GetLastError();
}

// 使用 ZwCreateThreadEx 实现远线程注入
BOOL DLLUtils::ZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName)
{
	HANDLE hProcess = NULL;
	SIZE_T dwSize = 0;
	LPVOID pDllAddr = NULL;
	FARPROC pFuncProcAddr = NULL;
	HANDLE hRemoteThread = NULL;
	DWORD dwStatus = 0;

	// 打开注入进程，获取进程句柄
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		printf("OpenProcess");
		return FALSE;
	}
	// 在注入进程中申请内存
	dwSize = 1 + ::strlen(pszDllFileName);
	pDllAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == pDllAddr)
	{
		printf("VirtualAllocEx");
		return FALSE;
	}
	// 向申请的内存中写入数据
	if (FALSE == ::WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL))
	{
		printf("WriteProcessMemory");
		return FALSE;
	}
	// 加载 ntdll.dll
	HMODULE hNtdllDll = ::LoadLibraryA(NT);
	if (NULL == hNtdllDll)
	{
		printf("LoadLirbary");
		return FALSE;
	}
	// 获取LoadLibraryA函数地址
	pFuncProcAddr = ::GetProcAddress(::GetModuleHandleA(KERNEL), "LoadLibraryA");
	if (NULL == pFuncProcAddr)
	{
		printf("GetProcAddress_LoadLibraryA");
		return FALSE;
	}
	// 获取ZwCreateThread函数地址
	typedef_ZwCreateThreadEx ZwCreateThreadEx = (typedef_ZwCreateThreadEx)::GetProcAddress(hNtdllDll, "ZwCreateThreadEx");
	if (NULL == ZwCreateThreadEx)
	{
		printf("GetProcAddress_ZwCreateThread");
		return FALSE;
	}
	// 使用 ZwCreateThreadEx 创建远线程, 实现 DLL 注入
	dwStatus = ZwCreateThreadEx(&hRemoteThread, PROCESS_ALL_ACCESS, NULL, hProcess, (LPTHREAD_START_ROUTINE)pFuncProcAddr, pDllAddr, 0, 0, 0, 0, NULL);
	if (NULL == hRemoteThread)
	{
		printf("ZwCreateThreadEx");
		return FALSE;
	}
	// 关闭句柄
	::CloseHandle(hProcess);
	::FreeLibrary(hNtdllDll);

	return TRUE;
}



LPVOID DLLUtils::Malloc(HANDLE hProcess, SIZE_T nSize)
{
	return VirtualAllocEx(hProcess, NULL, nSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}
BOOL DLLUtils::WriteMemory(HANDLE hProcess, LPVOID to, LPVOID from, SIZE_T nSize)
{
	return WriteProcessMemory(hProcess, to, from, nSize, NULL);
}
HMODULE DLLUtils::LoadDLL(const TCHAR* dPath)
{
	return LoadLibrary(dPath);
}
FARPROC DLLUtils::GetProc(HMODULE hDLL, const char* procName)
{
	return GetProcAddress(hDLL, procName);
}
HMODULE DLLUtils::GetModule(const TCHAR* mName)
{
	//只检查主调进程的地址空间
	return GetModuleHandle(mName);
}
stdstr DLLUtils::_GetModuleFileName(HMODULE hModule)
{
	TCHAR buf[MAX_PATH] = { 0 };
	DWORD dwRet = GetModuleFileName(hModule, buf, _countof(buf));
	return buf;
}
HMODULE DLLUtils::GetModuleEx(PCTSTR func)
{
	//输入函数入口地址，传出函数所在的dll基地址
	HMODULE handle;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, func, &handle);
	return handle;
}

BOOL DLLUtils::CreateDLL(const HANDLE& hToken, const stdstr& dllPath,STARTUPINFO& si, PROCESS_INFORMATION& pi)
{
	BOOL bRet = TRUE;
	HANDLE hDuplicatedToken = NULL;
	LPVOID lpEnvironment = NULL;
	do
	{
		// 复制令牌
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			bRet = FALSE;
			break;
		}

		// 创建用户Session环境
		if (FALSE == ::CreateEnvironmentBlock(&lpEnvironment,
			hDuplicatedToken, FALSE))
		{
			bRet = FALSE;
			break;
		}
		DWORD creationFlags = NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT;
		stdstr commandStr = PathHelper::GetSysFolder() + _T("\\regsvr32.exe ") + dllPath;
		TCHAR command[1024] = { 0 };
		lstrcpyn(command, commandStr.c_str(), 1024);
		bRet = CreateProcessAsUser(hDuplicatedToken, NULL,
			command,
			NULL, NULL, FALSE,
			creationFlags, lpEnvironment, NULL, &si, &pi);
	} while (FALSE);
	// 关闭句柄, 释放资源
	if (lpEnvironment)
	{
		::DestroyEnvironmentBlock(lpEnvironment);
		lpEnvironment = NULL;
	}
	if (hDuplicatedToken)
	{
		::CloseHandle(hDuplicatedToken);
		hDuplicatedToken = NULL;
	}
	return bRet;
}
BOOL DLLUtils::DisguiseProcess(DWORD dwProcessID, TCHAR* lpwszPath, TCHAR* lpwszCmd)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL)
	{
		return FALSE;
	}
	typedef_NtQueryInformationProcess NtQueryProcess = NULL;
	//获取函数地址
	NtQueryProcess = (typedef_NtQueryInformationProcess)GetProcAddress(LoadLibrary(_T(NT)), "NtQueryInformationProcess");
	if (NtQueryProcess == NULL)
	{
		return FALSE;
	}

	//获取指定进程基本信息
	//pbi中的peb指针指向的内存在对方进程，要去对方进程才能读到
	PROCESS_BASIC_INFORMATION pbi = { 0 };
	NTSTATUS status = NtQueryProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	//获取指定进程基本信息结构中的PebBaseAddress
	PEB peb = { 0 };
	ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL);
	//获取指定进程环境块结构中的ProcessParameters，该指针指向对方进程地址
	RTL_USER_PROCESS_PARAMETERS param = { 0 };
	ReadProcessMemory(hProcess, peb.ProcessParameters, &param, sizeof(param), NULL);

	USHORT usCmdLen = sizeof(TCHAR) * _tcslen(lpwszCmd) + sizeof(TCHAR);
	USHORT usPathLen = sizeof(TCHAR) * _tcslen(lpwszPath) + sizeof(TCHAR);

	//修改指定PEB中的命令行信息，注意该指针指向对方进程地址
	WriteProcessMemory(hProcess, param.CommandLine.Buffer, lpwszCmd, usCmdLen, NULL);
	//修改指定PEB中的路径信息，注意该指针指向对方进程地址
	WriteProcessMemory(hProcess, param.ImagePathName.Buffer, lpwszPath, usPathLen, NULL);
	WriteProcessMemory(hProcess, &param.ImagePathName.Length, &usPathLen, sizeof(usPathLen), NULL);
	CloseHandle(hProcess);
	return TRUE;
}

BOOL DLLUtils::ReplaceProcess(const TCHAR *pszFilePath, PVOID pReplaceData, 
	DWORD dwReplaceDataSize, DWORD dwRunOffset)
{
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = { 0 };
	//创建后挂起进程
	BOOL bRet = CreateProcess(pszFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (!bRet)
	{
		return bRet;
	}
	//申请一块可读可写内存
	LPVOID lpDestBaseAddr = Malloc(pi.hProcess, dwReplaceDataSize);
	if (!lpDestBaseAddr)
	{
		return FALSE;
	}
	//写入ShellCode数据
	bRet = WriteMemory(pi.hProcess, lpDestBaseAddr, pReplaceData, dwReplaceDataSize);
	if (!bRet)
	{
		return bRet;
	}
	//获取主线程上下文
	CONTEXT threadContext = { 0 };
	bRet = GetThreadContext(pi.hThread, &threadContext);
	if (!bRet)
	{
		return bRet;
	}
	//修改进程中的PE文件入口地址以及映像大小
	//获取原进程PE的加载基础地址
#ifdef _WIN64
	threadContext.Rip = (DWORD)lpDestBaseAddr + dwRunOffset;
#else
	threadContext.Eip = (DWORD)lpDestBaseAddr + dwRunOffset;
#endif
	//设置挂起线程上下文
	bRet = SetThreadContext(pi.hThread, &threadContext);
	if (!bRet)
	{
		return FALSE;
	}
	//恢复挂起的线程
	ResumeThread(pi.hThread);
	return TRUE;
}