#include<stdio.h>
#include"LogManager.hpp"
#include"PathHelper.hpp"
#include"../DLL注入/DLLUtils.h"
#include<Windows.h>

//充当regsvr32的功能
void main(int argc, _TCHAR* argv[])
{
	// 加载DLL并获取句柄
	HMODULE hDll = ::LoadLibrary(_T("HookAPI.dll"));
	if (NULL == hDll)
	{
		printf("%s error[%d]\n", "LoadLibrary", ::GetLastError());
		return;
	}
	printf("Load Library OK.\n");

	// 设置全局钩子
	typedef HHOOK(*typedef_SetHook)();
	typedef_SetHook SetHook = (typedef_SetHook)::GetProcAddress(hDll, "SetHook");
	if (NULL == SetHook)
	{
		printf("GetProcAddress Error[%d]\n", ::GetLastError());
	}
	HHOOK hHook = SetHook();
	if (NULL == hHook)
	{
		printf("%s error[%d]\n", "SetWindowsHookEx", ::GetLastError());
	}
	printf("Set Windows Hook OK.\n");
	system("pause");
	// 卸载全局钩子
	typedef BOOL(*typedef_UnsetHook)(HHOOK);
	typedef_UnsetHook UnsetHook = (typedef_UnsetHook)::GetProcAddress(hDll, "UnsetHook");
	if (NULL == UnsetHook)
	{
		printf("GetProcAddress Error[%d]\n", ::GetLastError());
	}
	if (FALSE == UnsetHook(hHook))
	{
		printf("%s error[%d]\n", "UnhookWindowsHookE", ::GetLastError());
	}
	printf("Unhook Windows Hook OK.\n");
	// 卸载DLL
	::FreeLibrary(hDll);

	system("pause");
}

#define PID 10396
#define DLL_PATH "D:\\Space\\System\\Windows\\DLLProject\\HookAPI\\x64\\Release\\HookAPI.dll"
//定向钩 远线程注入
void main2(int argc, _TCHAR* argv[])
{
	// 加载DLL并获取句柄
	HMODULE hDll = ::LoadLibrary(_T(DLL_PATH));
	if (NULL == hDll)
	{
		printf("%s error[%d]\n", "LoadLibrary", ::GetLastError());
		return;
	}
	printf("Load Library OK.\n");
	DLLUtils::InjectDLLThread(PID, DLL_PATH);
	printf("Hook OK.\n");
	system("pause");
	// 卸载DLL
	DLLUtils::FreeDLL(PID, TEXT(DLL_PATH));
	::FreeLibrary(hDll);

	system("pause");
}