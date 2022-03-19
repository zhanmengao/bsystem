#include"../DLL注入/HookUtils.h"
#include<stdio.h>
#include"LogManager.hpp"
#include"../DLL注入/ExpDefine.h"
#include<tchar.h>

// 共享内存
#pragma data_seg("HOOK_DATA")
HHOOK g_hHook = NULL;
LogManager logMgr;
#pragma data_seg()
#pragma comment(linker, "/SECTION:HOOK_DATA,RWS")


extern HMODULE g_hDllModule;

extern "C" __declspec(dllexport) VOID DllRegisterServer();
LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}
// 设置全局钩子
HHOOK SetHook()
{
	// 设置全局钩子
	HHOOK hHook = HookUtils::RegMessageHook(HookProc, g_hDllModule);
	if (!hHook)
	{
		logMgr.Log("RegMessageHook Error");
		return NULL;
	}
	else
	{
		logMgr.Log("RegMessageHook Sec!");
	}
	g_hHook = hHook;
	return hHook;
}
VOID DllRegisterServer()
{
	HHOOK hook = SetHook();
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			DispatchMessage(&msg);
		}
	}
	HookUtils::DisMessageHook(hook);
}

_MessageBoxA oldMsgBox;						//接收旧的函数入口
_CreateProcessAsUserW oldCreateProcess;					//旧的process函数
int WINAPI _MessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	logMgr.Log(string("messagebox ") + lpText);
	return oldMsgBox(hWnd, lpText, lpCaption, uType);
}
int WINAPI _CreateProcessAsUser(_In_opt_ HANDLE hToken,
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation)
{
	logMgr.Log("_CreateProcessAsUser ");
	MessageBox(NULL, _T("AAA"), +_T("GGG"), MB_OK);
	return oldCreateProcess(hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags,
		lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}
//注入后每个进程都会触发dllmain，在这里实现HOOK
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvREserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		logMgr.SetPath("D:\\logHook.txt");
		g_hDllModule = hinstDLL;
		MH_STATUS status = HookUtils::HookFunction(&CreateProcessAsUserW,
			&_CreateProcessAsUser, (void**)&oldCreateProcess);
		if (status == MH_STATUS::MH_OK)
		{
			logMgr.Log("HookFunction CreateProcessAsUserW Sec!");
		}
		else
		{
			logMgr.Log("HookFunction CreateProcessAsUserW Fail " + to_string(status));
		}
		status = HookUtils::HookFunction(&MessageBoxA, &_MessageBox, (void**)&oldMsgBox);
		if (status == MH_STATUS::MH_OK)
		{
			logMgr.Log("HookFunction MessageBoxW Sec!");
		}
		else
		{
			logMgr.Log("HookFunction MessageBoxW Fail!" + to_string(status));
		}
	}
	break;
	case DLL_PROCESS_DETACH:
		HookUtils::UnHookFunction(&CreateProcessAsUserW);
		HookUtils::UnHookFunction(&MessageBoxA);
		HookUtils::UnInitialize();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

