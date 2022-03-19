#pragma once
#include<Windows.h>
#include "tlhelp32.h"
#include"ExpDefine.h"

#define LOAD "LoadLibrary"
#define FREE "FreeLibrary"
#define PUBLIC_DLL "kernel32.dll"

#include"Convert.h"
void InjectDLL(DWORD pid, const char* dllPath)
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
		FARPROC pFuncAddr = GetProcAddress(GetModuleHandleA(PUBLIC_DLL), LOAD);
		if (pFuncAddr == nullptr)
		{
			break;
		}
		//Զ�̴����߳�
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)pFuncAddr, pDllAddr, NULL, NULL);
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

void FreeDLL(DWORD pid, const char* inDllPath)
{
	if (pid == NULL || strlen(inDllPath) == 0)
	{
		return;
	}
	HANDLE hSnap = INVALID_HANDLE_VALUE;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	char dllPath[MAX_PATH] = { 0 };
	strcpy(dllPath, inDllPath);
	char szExePath[MAX_PATH] = { 0 };
	do
	{
		//����dllģ��
		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		MODULEENTRY32 me32 = { 0 };
		me32.dwSize = sizeof(me32);
		BOOL bRet = Module32First(hSnap, &me32);
		while (bRet)
		{
			memset(szExePath, 0, sizeof(szExePath));
			strcpy(szExePath, Convert::WStringToString(me32.szExePath).c_str());
			if (strcmp(strupr(szExePath), strupr(dllPath)) == 0)
			{
					break;
			}
			else
			{
				bRet = Module32Next(hSnap, &me32);
			}
		}

		//��Ŀ�����
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProcess == INVALID_HANDLE_VALUE)
		{
			break;
		}
		//�õ��Զ˺�����ڵ�ַ��kernel32���κλ�����ַ��һ�£�
		FARPROC pFuncAddr = GetProcAddress(GetModuleHandleA(PUBLIC_DLL), FREE);
		if (pFuncAddr == nullptr)
		{
			break;
		}
		//Զ�̴����̵߳���free
		hThread = CreateRemoteThread(hProcess, NULL, 0,
			(LPTHREAD_START_ROUTINE)pFuncAddr, me32.hModule, NULL, NULL);
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
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSnap);
		hSnap = INVALID_HANDLE_VALUE;
	}
}

DWORD WINAPI RemoteThreadProc(LPVOID lpParam)
{
	_InjectDATA* pData = (_InjectDATA*)lpParam;

	//�Ը�������ַ���и�ֵ
	_LoadLibraryA load =(_LoadLibraryA) pData->dwLoadLibrary;
	_GetProcAddress proc = (_GetProcAddress)pData->dwGetProcAddress;
	_GetModuleHandleA modulehand = (_GetModuleHandleA)pData->dwGetModuleHandle;
	_GetModuleFileNameA filename = (_GetModuleFileNameA)pData->dwGetModuleFileName;

	//����User32.dll
	HMODULE hModule = load(pData->User32Dll);
	//���MessageBoxA��ַ
	_MessageBoxA msgA = (_MessageBoxA)proc(hModule, pData->MsgBox);
	char szModuleFileName[MAX_PATH] = { 0 };
	filename(modulehand(NT), szModuleFileName, MAX_PATH);
	msgA(NULL, pData->text, szModuleFileName, MB_OK);
}