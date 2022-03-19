//使用命令注册的DLL类型属于组件式的DLL文件
//组件式的DLL文件与普通的DLL文件不同之处在于默认导出四个函数，
//在编程的时候可以看到这四个导出函数DllGetClassObject，DllRegisterServer，DllUnregisterServer和DllMain
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
//获取到当前用户路径
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