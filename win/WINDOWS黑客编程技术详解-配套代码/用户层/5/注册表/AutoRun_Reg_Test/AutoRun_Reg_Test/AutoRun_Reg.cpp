#include "stdafx.h"
#include "AutoRun_Reg.h"


void ShowError(char *lpszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error!\nError Code Is:%d\n", lpszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


BOOL Reg_CurrentUser(char *lpszFileName, char *lpszValueName)
{
	// Ĭ��Ȩ��
	HKEY hKey;
	// ��ע����
	if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey))
	{
		ShowError("RegOpenKeyEx");
		return FALSE;
	}
	// �޸�ע���ֵ��ʵ�ֿ�������
	if (ERROR_SUCCESS != ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ, (BYTE *)lpszFileName, (1 + ::lstrlen(lpszFileName))))
	{
		::RegCloseKey(hKey);
		ShowError("RegSetValueEx");
		return FALSE;
	}
	// �ر�ע����
	::RegCloseKey(hKey);

	return TRUE;
}


BOOL Reg_LocalMachine(char *lpszFileName, char *lpszValueName)
{
	// ����ԱȨ��
	HKEY hKey;
	// ��ע����
	if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey))
	{
		ShowError("RegOpenKeyEx");
		return FALSE;
	}
	// �޸�ע���ֵ��ʵ�ֿ�������
	if (ERROR_SUCCESS != ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ, (BYTE *)lpszFileName, (1 + ::lstrlen(lpszFileName))))
	{
		::RegCloseKey(hKey);
		ShowError("RegSetValueEx");
		return FALSE;
	}
	// �ر�ע����
	::RegCloseKey(hKey);

	return TRUE;
}