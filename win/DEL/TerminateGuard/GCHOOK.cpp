// GCHOOK.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#define  GCHOOK_EXPORTS
#include "GCHOOK.h"


#pragma data_seg(".Hook")
HHOOK g_GetMsgHook = NULL;
#pragma data_seg()

#pragma comment(linker,"/section:.Hook,rws")

LRESULT CALLBACK GetMsgProc( int nCode,	WPARAM wParam,LPARAM lParam )
{
	return CallNextHookEx( g_GetMsgHook, nCode, wParam, lParam);
}

GCHOOK_API void StartMonitor()
{
	if (!g_GetMsgHook)
	{


#if defined _M_X64
		//ProcessGuard_x64.dll
		g_GetMsgHook = SetWindowsHookEx(WH_GETMESSAGE,
			GetMsgProc,
			GetModuleHandleA("TerminateGuard_x64.dll"),
			0);
#elif defined _M_IX86
		//ProcessGuard_Win32.dll
		g_GetMsgHook = SetWindowsHookEx(WH_GETMESSAGE,
			GetMsgProc,
			GetModuleHandleA("TerminateGuard_Win32.dll"),
			0);
#endif

	}
}

GCHOOK_API void StopMonitor()
{
	if (g_GetMsgHook)
	{
		if (UnhookWindowsHookEx(g_GetMsgHook))
		{
			g_GetMsgHook = NULL;
		}
		else
		{
			UnhookWindowsHookEx(g_GetMsgHook);
			g_GetMsgHook = NULL;
		}
	}
}
