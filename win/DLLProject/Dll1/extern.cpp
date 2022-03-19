#include<Windows.h>

extern "C" __declspec(dllexport) VOID MsgBox(WCHAR* szMsg);

VOID MsgBox(WCHAR* szMsg)
{
	WCHAR szModuleName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szModuleName, MAX_PATH);
	MessageBox(NULL, szMsg, szModuleName, MB_OK);
}