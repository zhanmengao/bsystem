//ʹ������ע���DLL�����������ʽ��DLL�ļ�
//���ʽ��DLL�ļ�����ͨ��DLL�ļ���֮ͬ������Ĭ�ϵ����ĸ�������
//�ڱ�̵�ʱ����Կ������ĸ���������DllGetClassObject��DllRegisterServer��DllUnregisterServer��DllMain
#include <stdio.h>
#include <shlobj.h>
#include <string>
#include<wchar.h>
#include<Windows.h>
#include<tchar.h>
using namespace std;
#pragma   comment(lib,   "shell32.lib")
/*
DWORD UsernamePathSize = ARRAYSIZE(szUsernamePath);
//��ȡ����ǰ�û�·��
if (!GetUserProfileDirectory(hToken, szUsernamePath, &UsernamePathSize))
{
log_error("%s GetUserProfileDirectory is failed error:% i\n", __FUNCTION__, GetLastError());
}
*/
extern "C" __declspec(dllexport) VOID DllRegisterServer();
#define SYSTEM_BEGIN "C:\\WINDOWS"
wstring GetCurrentUserPath()
{
	TCHAR szChar[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szChar, CSIDL_APPDATA, FALSE);
	for (int i = 0; i < MAX_PATH; i++)
	{
		szChar[i] = toupper(szChar[i]);
	}
	return szChar;
}
bool IsSystem()
{
	wstring str = GetCurrentUserPath();
	if (str.substr(0, strlen(SYSTEM_BEGIN) + 1) == _T(SYSTEM_BEGIN))
	{
		return true;
	}
	else
	{
		return false;
	}
}
VOID DllRegisterServer()
{
	MessageBox(NULL, L"DllRegisterServer Sec", L"DllRegisterServer", MB_OK);
	if(IsSystem())
	{
		MessageBox(NULL, L"IsSystem() is true", L"", MB_OK);
	}
	else
	{
		MessageBox(NULL, L"IsSystem() is false", L"", MB_OK);
	}
}