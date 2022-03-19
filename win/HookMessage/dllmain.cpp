// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include<Windows.h>
#include"../DLL注入/HookUtils.h"
#include"../DLL注入/ExpDefine.h"

extern HMODULE g_hModule;
#define EXPORT extern "C" __declspec(dllexport)
#pragma data_seg("mydata")
HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:mydata,RWS")

EXPORT HHOOK SetHook();
EXPORT BOOL UnsetHook(HHOOK hHook);
HMODULE g_hModule;
// 消息全局钩子回调函数
LRESULT CALLBACK GetMsgProc(
	int code,       // hook code
	WPARAM wParam,  // removal option  
	LPARAM lParam   // message
)
{
	// 不进行任何操作，设置全局钩子的目的就是进行DLL注入而已，主要是主入口进行的API挂钩
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}
_MessageBoxW oldMsgBoxW;						//接收旧的函数入口
_MessageBoxA oldMsgBoxA;						//接收旧的函数入口
int WINAPI MyMessageBoxW(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType)
{
	OutputDebugStringA("MyMessageBoxW Call");
	WCHAR buffer[1024] = { 0 };
	wsprintfW(buffer, L"MyWTSSendMessageW title = %s message = %s", lpCaption, lpText);
	OutputDebugStringW(buffer);
	if (wcscmp(lpCaption, L"空闲计时器已过期") == 0)
	{
		return FALSE;
	}
	return oldMsgBoxW(hWnd, lpText, lpCaption, uType);
}
int WINAPI MyMessageBoxA(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType)
{
	OutputDebugStringA("MyMessageBoxA Call");
	char buffer[1024] = { 0 };
	wsprintfA(buffer, "MyWTSSendMessageA title = %s message = %s", lpCaption, lpText);
	OutputDebugStringA(buffer);
	if (strcmp(lpCaption, "空闲计时器已过期") == 0)
	{
		return FALSE;
	}
	return oldMsgBoxA(hWnd, lpText, lpCaption, uType);
}
// 设置全局钩子
HHOOK SetHook()
{
	// 设置全局钩子
	HHOOK hHook = HookUtils::RegMessageHook(GetMsgProc, g_hModule);
	if (NULL == hHook)
	{
		return NULL;
	}
	g_hHook = hHook;
	return hHook;
}


// 卸载全局钩子
BOOL UnsetHook(HHOOK hHook)
{
	return HookUtils::DisMessageHook(hHook);
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HookUtils::HookFunction(&MessageBoxW, &MyMessageBoxW, (void**)&oldMsgBoxW);
		HookUtils::HookFunction(&MessageBoxA, &MyMessageBoxA, (void**)&oldMsgBoxA);
		g_hModule = hModule;
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		break;
	}
	case DLL_THREAD_DETACH:
	{
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		HookUtils::UnHookFunction(&MessageBoxW);
		HookUtils::UnHookFunction(&MessageBoxA);
		HookUtils::UnInitialize();
		break;
	}
	default:
		break;
	}
	return TRUE;
}


