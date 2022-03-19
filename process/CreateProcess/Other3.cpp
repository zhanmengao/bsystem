#include <windows.h>
#include <stdint.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <iostream>
#include <vector>

typedef struct EnumHWndsArg
{
	std::vector<HWND> *vecHWnds;
	DWORD dwProcessId;
}EnumHWndsArg, *LPEnumHWndsArg;

HANDLE GetProcessHandleByID(int nID)//通过进程ID获取进程句柄
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, nID);
}

DWORD GetProcessIDByName(const WCHAR* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
		if (lstrcmp(pe.szExeFile, pName) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
		//printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
	}
	CloseHandle(hSnapshot);
	return 0;
}

BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	EnumHWndsArg *pArg = (LPEnumHWndsArg)lParam;
	DWORD  processId;
	GetWindowThreadProcessId(hwnd, &processId);
	if (processId == pArg->dwProcessId)
	{
		pArg->vecHWnds->push_back(hwnd);
		//printf("%p\n", hwnd);
	}
	return TRUE;
}

void GetHWndsByProcessID(DWORD processID, std::vector<HWND> &vecHWnds)
{
	EnumHWndsArg wi;
	wi.dwProcessId = processID;
	wi.vecHWnds = &vecHWnds;
	EnumWindows(lpEnumFunc, (LPARAM)&wi);
}

int32_t maino4()
{
	DWORD pid = GetProcessIDByName(L"notepad++.exe");
	printf("pid = %u\n", pid);
	if (pid != 0)
	{
		std::vector<HWND> vecHWnds;
		GetHWndsByProcessID(pid, vecHWnds);
		printf("vecHWnds.size() = %u\n", vecHWnds.size());
		for (const HWND &h : vecHWnds)
		{
			HWND parent = GetParent(h);
			if (parent == NULL)
			{
				printf("%p --->Main Wnd\n", h);
			}
			else
			{
				printf("%p %p\n", h, parent);
			}
		}
	}
	getchar();
	return S_OK;
}