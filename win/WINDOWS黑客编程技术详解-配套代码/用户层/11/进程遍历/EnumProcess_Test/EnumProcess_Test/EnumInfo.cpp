#include "stdafx.h"
#include "EnumInfo.h"


void ShowError(char *lpszText)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error[%d]\n", lpszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
#endif
}


BOOL EnumProcess()
{
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// ��ȡȫ�����̿���
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap)
	{
		ShowError("CreateToolhelp32Snapshot");
		return FALSE;
	}

	// ��ȡ�����е�һ����Ϣ
	BOOL bRet = ::Process32First(hProcessSnap, &pe32);
	while (bRet)
	{
		// ��ʾ Process ID
		printf("[%d]\t", pe32.th32ProcessID);

		// ��ʾ ��������
		printf("[%s]\n", pe32.szExeFile);

		// ��ȡ��������һ����Ϣ
		bRet = ::Process32Next(hProcessSnap, &pe32);
	}

	// �رվ��
	::CloseHandle(hProcessSnap);

	return TRUE;
}


BOOL EnumThread()
{
	THREADENTRY32 te32 = { 0 };
	te32.dwSize = sizeof(THREADENTRY32);
	// ��ȡȫ���߳̿���
	HANDLE hThreadSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hThreadSnap)
	{
		ShowError("CreateToolhelp32Snapshot");
		return FALSE;
	}

	// ��ȡ�����е�һ����Ϣ
	BOOL bRet = ::Thread32First(hThreadSnap, &te32); 
	while (bRet)
	{
		// ��ʾ Owner Process ID
		printf("[%d]\t", te32.th32OwnerProcessID);

		// ��ʾ Thread ID
		printf("[%d]\n", te32.th32ThreadID);

		// ��ȡ��������һ����Ϣ
		bRet = ::Thread32Next(hThreadSnap, &te32);
	}

	// �رվ��
	::CloseHandle(hThreadSnap);

	return TRUE;
}


BOOL EnumProcessModule(DWORD dwProcessId)
{
	MODULEENTRY32 me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32);
	// ��ȡָ������ȫ��ģ��Ŀ���
	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (INVALID_HANDLE_VALUE == hModuleSnap)
	{
		ShowError("CreateToolhelp32Snapshot");
		return FALSE;
	}

	// ��ȡ�����е�һ����Ϣ
	BOOL bRet = ::Module32First(hModuleSnap, &me32);
	while (bRet)
	{
		// ��ʾ Process ID
		printf("[%d]\t", me32.th32ProcessID);

		// ��ʾ ģ����ػ�ַ
		printf("[0x%p]\t", me32.modBaseAddr);

		// ��ʾ ģ������
		printf("[%s]\n", me32.szModule);

		// ��ȡ��������һ����Ϣ
		bRet = ::Module32Next(hModuleSnap, &me32);
	}

	// �رվ��
	::CloseHandle(hModuleSnap);

	return TRUE;
}