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
		//��Ŀ�����
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}

		//��Ŀ����������ڴ�
		unsigned dllLen = strlen(dllPath) + sizeof(char);
		PVOID pDllAddr = VirtualAllocEx(hProcess, NULL, dllLen, MEM_COMMIT, PAGE_READWRITE);
		if (pDllAddr == nullptr)
		{
			break;
		}

		//��Ŀ������ڴ�дֵ
		SIZE_T dwWriteNum = 0;
		WriteProcessMemory(hProcess, pDllAddr, dllPath, dllLen, &dwWriteNum);
		
		//�õ��Զ˺�����ڵ�ַ��kernel32���κλ�����ַ��һ�£�
		FARPROC pFuncAddr = GetProcAddress(GetModuleHandleA(KERNEL), LOAD);
		if (pFuncAddr == nullptr)
		{
			break;
		}
		//Զ�̴����߳�
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
		//��Ŀ�����
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}

		//��ַ��ͬ������ע��ǰ��ȡ
		_InjectDATA data;
		data.dwLoadLibrary = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), LOAD);
		data.dwGetProcAddress = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), "GetProcAddress");
		data.dwGetModuleHandle = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), "GetModuleHandleA");
		data.dwGetModuleFileName = (DWORD)GetProcAddress(GetModule(_T(KERNEL)), "GetModuleFileNameA");
		strcpy_s(data.User32Dll,_countof(data.User32Dll),"user32.dll");
		strcpy_s(data.MsgBox, _countof(data.MsgBox), "MessageBoxA");
		strcpy_s(data.text, _countof(data.text), "Inject Code!!!");
		//��Ŀ����������ڴ棬д��ṹ��
		PVOID lpData = VirtualAllocEx(hProcess, NULL, sizeof(data), MEM_COMMIT, PAGE_READWRITE);
		if (lpData == nullptr)
		{
			break;
		}
		SIZE_T dwWriteNum = 0;
		WriteProcessMemory(hProcess, lpData, &data, sizeof(data), &dwWriteNum);

		//��Ŀ����������ڴ棬д������
		DWORD dwFunSize = 0x4000;
		PVOID lpCode = VirtualAllocEx(hProcess, NULL, dwFunSize, MEM_COMMIT, PAGE_READWRITE);
		if (lpCode == nullptr)
		{
			break;
		}
		//��Ŀ������ڴ�дֵ
		WriteProcessMemory(hProcess, lpCode, &proc, dwFunSize, &dwWriteNum);

		//Զ�̴����߳�
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
void DLLUtils::InjectDLLAPC(DWORD pid, const char* dllPath)							//��DLLע��ָ������
{
	if (pid == 0 || strlen(dllPath) == 0)
	{
		return;
	}
	//����dll�ļ�·������
	int nDllLen = strlen(dllPath) + sizeof(char);

	//��Ŀ�����
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if (hProcess == NULL)
	{
		return;
	}
	//��Ŀ����������ڴ�
	PVOID pDllAddr = Malloc(hProcess, nDllLen);
	if (pDllAddr == NULL)
	{
		CloseHandle(hProcess);
		return;
	}
	DWORD dwWriteNum = 0;
	//д���ڴ�
	WriteMemory(hProcess, pDllAddr, (void*)dllPath, nDllLen);
	CloseHandle(hProcess);

	//�õ��߳̿���
	THREADENTRY32 te32 = { 0 };
	te32.dwSize = sizeof(te32);
	HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == handleSnap)
	{
		CloseHandle(hProcess);
		return;
	}
	//��ȡLoad������ڵ�ַ
	FARPROC pFuncAddr = GetProcAddress(GetModule(_T(KERNEL)), LOAD);

	//�õ���һ���߳�
	DWORD dwRet = 0;
	if (Thread32First(handleSnap, &te32))
	{
		do
		{
			//����id�Ա�
			if (te32.th32OwnerProcessID == pid)
			{
				//�õ��߳̾��
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
	//�������н��̣�������dll
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
		//����dllģ��
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
		//��Ŀ�����
		hProcess = OpenProcess(PROCESS_CREATE_THREAD, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}
		//�õ��Զ˺�����ڵ�ַ��kernel32���κλ�����ַ��һ�£�
		FARPROC pFuncAddr = GetProcAddress(GetModuleHandleA(KERNEL), FREE);
		if (pFuncAddr == nullptr)
		{
			break;
		}
		//Զ�̴����̵߳���free
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

// ʹ�� ZwCreateThreadEx ʵ��Զ�߳�ע��
BOOL DLLUtils::ZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName)
{
	HANDLE hProcess = NULL;
	SIZE_T dwSize = 0;
	LPVOID pDllAddr = NULL;
	FARPROC pFuncProcAddr = NULL;
	HANDLE hRemoteThread = NULL;
	DWORD dwStatus = 0;

	// ��ע����̣���ȡ���̾��
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		printf("OpenProcess");
		return FALSE;
	}
	// ��ע������������ڴ�
	dwSize = 1 + ::strlen(pszDllFileName);
	pDllAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == pDllAddr)
	{
		printf("VirtualAllocEx");
		return FALSE;
	}
	// ��������ڴ���д������
	if (FALSE == ::WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL))
	{
		printf("WriteProcessMemory");
		return FALSE;
	}
	// ���� ntdll.dll
	HMODULE hNtdllDll = ::LoadLibraryA(NT);
	if (NULL == hNtdllDll)
	{
		printf("LoadLirbary");
		return FALSE;
	}
	// ��ȡLoadLibraryA������ַ
	pFuncProcAddr = ::GetProcAddress(::GetModuleHandleA(KERNEL), "LoadLibraryA");
	if (NULL == pFuncProcAddr)
	{
		printf("GetProcAddress_LoadLibraryA");
		return FALSE;
	}
	// ��ȡZwCreateThread������ַ
	typedef_ZwCreateThreadEx ZwCreateThreadEx = (typedef_ZwCreateThreadEx)::GetProcAddress(hNtdllDll, "ZwCreateThreadEx");
	if (NULL == ZwCreateThreadEx)
	{
		printf("GetProcAddress_ZwCreateThread");
		return FALSE;
	}
	// ʹ�� ZwCreateThreadEx ����Զ�߳�, ʵ�� DLL ע��
	dwStatus = ZwCreateThreadEx(&hRemoteThread, PROCESS_ALL_ACCESS, NULL, hProcess, (LPTHREAD_START_ROUTINE)pFuncProcAddr, pDllAddr, 0, 0, 0, 0, NULL);
	if (NULL == hRemoteThread)
	{
		printf("ZwCreateThreadEx");
		return FALSE;
	}
	// �رվ��
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
	//ֻ����������̵ĵ�ַ�ռ�
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
	//���뺯����ڵ�ַ�������������ڵ�dll����ַ
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
		// ��������
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			bRet = FALSE;
			break;
		}

		// �����û�Session����
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
	// �رվ��, �ͷ���Դ
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
	//��ȡ������ַ
	NtQueryProcess = (typedef_NtQueryInformationProcess)GetProcAddress(LoadLibrary(_T(NT)), "NtQueryInformationProcess");
	if (NtQueryProcess == NULL)
	{
		return FALSE;
	}

	//��ȡָ�����̻�����Ϣ
	//pbi�е�pebָ��ָ����ڴ��ڶԷ����̣�Ҫȥ�Է����̲��ܶ���
	PROCESS_BASIC_INFORMATION pbi = { 0 };
	NTSTATUS status = NtQueryProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	//��ȡָ�����̻�����Ϣ�ṹ�е�PebBaseAddress
	PEB peb = { 0 };
	ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL);
	//��ȡָ�����̻�����ṹ�е�ProcessParameters����ָ��ָ��Է����̵�ַ
	RTL_USER_PROCESS_PARAMETERS param = { 0 };
	ReadProcessMemory(hProcess, peb.ProcessParameters, &param, sizeof(param), NULL);

	USHORT usCmdLen = sizeof(TCHAR) * _tcslen(lpwszCmd) + sizeof(TCHAR);
	USHORT usPathLen = sizeof(TCHAR) * _tcslen(lpwszPath) + sizeof(TCHAR);

	//�޸�ָ��PEB�е���������Ϣ��ע���ָ��ָ��Է����̵�ַ
	WriteProcessMemory(hProcess, param.CommandLine.Buffer, lpwszCmd, usCmdLen, NULL);
	//�޸�ָ��PEB�е�·����Ϣ��ע���ָ��ָ��Է����̵�ַ
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
	//������������
	BOOL bRet = CreateProcess(pszFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (!bRet)
	{
		return bRet;
	}
	//����һ��ɶ���д�ڴ�
	LPVOID lpDestBaseAddr = Malloc(pi.hProcess, dwReplaceDataSize);
	if (!lpDestBaseAddr)
	{
		return FALSE;
	}
	//д��ShellCode����
	bRet = WriteMemory(pi.hProcess, lpDestBaseAddr, pReplaceData, dwReplaceDataSize);
	if (!bRet)
	{
		return bRet;
	}
	//��ȡ���߳�������
	CONTEXT threadContext = { 0 };
	bRet = GetThreadContext(pi.hThread, &threadContext);
	if (!bRet)
	{
		return bRet;
	}
	//�޸Ľ����е�PE�ļ���ڵ�ַ�Լ�ӳ���С
	//��ȡԭ����PE�ļ��ػ�����ַ
#ifdef _WIN64
	threadContext.Rip = (DWORD)lpDestBaseAddr + dwRunOffset;
#else
	threadContext.Eip = (DWORD)lpDestBaseAddr + dwRunOffset;
#endif
	//���ù����߳�������
	bRet = SetThreadContext(pi.hThread, &threadContext);
	if (!bRet)
	{
		return FALSE;
	}
	//�ָ�������߳�
	ResumeThread(pi.hThread);
	return TRUE;
}