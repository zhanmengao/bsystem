// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include "MinHook.h"
#include <stdio.h>

#include <string>


#include "ProcessExit.h"

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Advapi32.lib")

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

wchar_t*  MultiByteToWideChar2(const char* pKey);



using namespace std;


/**
* HOOK  API 原型声明
================================================================================ */

/***
* CreateProcess 创建进程 API
**/
typedef BOOL (WINAPI *oldCreateProcessW)(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation);

typedef BOOL (WINAPI *oldCreateProcessA)(
	_In_opt_ LPCSTR lpApplicationName,
	_Inout_opt_ LPSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOA lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
);

oldCreateProcessW pfnCreateProcessW = NULL;
oldCreateProcessA pfnCreateProcessA = NULL;

/***
* DialogBoxIndirectParam 调用模态对话框 API
**/
/**
typedef INT_PTR (WINAPI* oldDialogBoxIndirectParamA)(
		_In_opt_ HINSTANCE hInstance,
		_In_ LPCDLGTEMPLATEA hDialogTemplate,
		_In_opt_ HWND hWndParent,
		_In_opt_ DLGPROC lpDialogFunc,
		_In_ LPARAM dwInitParam);

typedef INT_PTR (WINAPI* oldDialogBoxIndirectParamW)(
		_In_opt_ HINSTANCE hInstance,
		_In_ LPCDLGTEMPLATEW hDialogTemplate,
		_In_opt_ HWND hWndParent,
		_In_opt_ DLGPROC lpDialogFunc,
		_In_ LPARAM dwInitParam);


oldDialogBoxIndirectParamA pfnDialogBoxIndirectParamA = NULL;
oldDialogBoxIndirectParamW pfnDialogBoxIndirectParamW = NULL;
**/
/***
* ExitProcess 结束进程 API
**/
/**
typedef VOID(WINAPI* oldExitProcess)(
	_In_ UINT uExitCode	);

oldExitProcess pfnExitProcess = NULL;
**/

/***
* TerminateProcess 结束进程 API
**/

/**
typedef BOOL(WINAPI* oldTerminateProcess)(
	IN HANDLE hServer,
	IN DWORD ProcessId,
	IN DWORD ExitCode);

oldTerminateProcess pfnTerminateProcess = NULL;
**/

/**
* 截获 API 后执行自己的处理函数
* ======================================================================= */

BOOL WINAPI myCreateProcessW(
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
	//判断进程是否在涉危列表中
	if (processGuard(lpCommandLine))
	{
		//直接返回，不再调用原生的进程创建方法，阻止进程被创建
		return true;
	}

	//调用原生的进程创建方法
	return pfnCreateProcessW( lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
	 
}

BOOL WINAPI myCreateProcessA(
	_In_opt_ LPCSTR lpApplicationName,
	_Inout_opt_ LPSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOA lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation)
{

	wchar_t* lpCommandLineW = MultiByteToWideChar2(lpCommandLine);

	if (processGuard(lpCommandLineW))
	{
		lpCommandLineW = NULL;
		delete lpCommandLineW;

		return true;
	}
	
	lpCommandLineW = NULL;
	delete lpCommandLineW;

	return pfnCreateProcessA(lpApplicationName,	lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);

}

/***
INT_PTR WINAPI myDialogBoxIndirectParamW(
	_In_opt_ HINSTANCE hInstance,
	_In_ LPCDLGTEMPLATEW hDialogTemplate,
	_In_opt_ HWND hWndParent,
	_In_opt_ DLGPROC lpDialogFunc,
	_In_ LPARAM dwInitPara)
{
			
	WCHAR outText[MAX_PATH] = { 0 };
	
	wchar_t modname[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, modname, sizeof(modname));
	
	wsprintf( outText,L"%ls :: DialogBoxIndirectParamW ==> style:%lu dwExtendedStyle::%lu c:%d y:%d cx:%d cy:%d cdit:%d",
		modname ,
		hDialogTemplate->style,
		hDialogTemplate->dwExtendedStyle,
		hDialogTemplate->x,
		hDialogTemplate->y,
		hDialogTemplate->cx,
		hDialogTemplate->cy,
		hDialogTemplate->cdit );
	

	OutputDebugString( outText );

	//DWORD errorStyleID1 = 2194145920;
	DWORD errorStyleID2 = 2194145408;
	

	// 阻止 Windows 的错误对话框弹出
	/* ======================================================== 
	* 本次操作由于这台计算机的限制而被取消请与系统管理员联系
	* ========================================================= */

/***
	if (   errorStyleID2 == hDialogTemplate->style &&
		   ( hDialogTemplate->cx == 10 && hDialogTemplate->cy == 10 )
		)
	{		
		OutputDebugString(L"Ignore Error DialogBox");
		return NULL;		
	}

	return pfnDialogBoxIndirectParamW(
		hInstance,
		hDialogTemplate,
		hWndParent,
		lpDialogFunc,
		dwInitPara);

}

INT_PTR WINAPI myDialogBoxIndirectParamA(
	_In_opt_ HINSTANCE hInstance,
	_In_ LPCDLGTEMPLATEA hDialogTemplate,
	_In_opt_ HWND hWndParent,
	_In_opt_ DLGPROC lpDialogFunc,
	_In_ LPARAM dwInitParam)
{

	OutputDebugString(L"DialogBoxIndirectParamA");

	return pfnDialogBoxIndirectParamA(
		hInstance,
		hDialogTemplate,
		hWndParent,
		lpDialogFunc,
		dwInitParam);

}

VOID WINAPI myExitProcess(
	_In_ UINT uExitCode
)
{
	DWORD ProcessId = GetCurrentProcessId();

	if (ProcessId  > 1)
	{
		QuitProcess(ProcessId,L"ExitProcess");
	}
	
	return  pfnExitProcess(uExitCode);
}

BOOL WINAPI myTerminateProcess(
	IN HANDLE hServer,
	IN DWORD ProcessId,
	IN DWORD ExitCode)
{
	
	if ( ProcessId > 1 )
	{		
		QuitProcess(ProcessId,L"TerminateProcess");
	}	

	return pfnTerminateProcess(
		hServer,
		ProcessId,
		ExitCode);
}
***/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL  HOOK()
{
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		OutputDebugString(TEXT("MH_Initialize Error"));
		return FALSE;
	}
	
	// Create a hook
	if (MH_CreateHook(
		&CreateProcessA,
		&myCreateProcessA,
		reinterpret_cast<void**>(&pfnCreateProcessA)) != MH_OK)
	{
		OutputDebugString(TEXT("MH_CreateHook CreateProcessA Error"));
		return FALSE;

	}
		
	if (MH_CreateHook(
		&CreateProcessW,
		&myCreateProcessW,
		reinterpret_cast<void**>(&pfnCreateProcessW)) != MH_OK)
	{
		OutputDebugString(TEXT("MH_CreateHook CreateProcessW Error"));
		return FALSE;
	}
/**		
	if (MH_CreateHook(
		&DialogBoxIndirectParamA,
		&myDialogBoxIndirectParamA,
		reinterpret_cast<void**>(&pfnDialogBoxIndirectParamA)) != MH_OK)
	{
		OutputDebugString(TEXT("MH_CreateHook DialogBoxIndirectParamA Error"));
		return FALSE;

	}

	if (MH_CreateHook(
		&DialogBoxIndirectParamW,
		&myDialogBoxIndirectParamW,
		reinterpret_cast<void**>(&pfnDialogBoxIndirectParamW)) != MH_OK)
	{
		OutputDebugString(TEXT("MH_CreateHook DialogBoxIndirectParamW Error"));
		return FALSE;

	}

	if (MH_CreateHook(
		&ExitProcess,
		&myExitProcess,
		reinterpret_cast<void**>(&pfnExitProcess)) != MH_OK)
	{
		OutputDebugString(TEXT("MH_CreateHook ExitProcess Error"));
		return FALSE;
	}

	
	if (MH_CreateHook(
		&TerminateProcess,
		&myTerminateProcess,
		reinterpret_cast<void**>(&pfnTerminateProcess)) != MH_OK)
	{
		OutputDebugString(TEXT("MH_CreateHook TerminateProcess Error"));
		return FALSE;
	}

**/
	// Enable the hook.	
	if (MH_EnableHook(&CreateProcessA) != MH_OK)
	{
		OutputDebugString(TEXT("MH_EnableHook CreateProcessA Error"));
		return FALSE;
	}
		
	if (MH_EnableHook(&CreateProcessW) != MH_OK)
	{
		OutputDebugString(TEXT("MH_EnableHook CreateProcessW Error"));
		return FALSE;
	}
/**	
	if (MH_EnableHook(&DialogBoxIndirectParamA) != MH_OK)
	{
		OutputDebugString(TEXT("MH_EnableHook DialogBoxIndirectParamA Error"));
		return FALSE;
	}

	if (MH_EnableHook(&DialogBoxIndirectParamW) != MH_OK)
	{
		OutputDebugString(TEXT("MH_EnableHook DialogBoxIndirectParamW Error"));
		return FALSE;
	}


	if (MH_EnableHook(&ExitProcess) != MH_OK)
	{
		OutputDebugString(TEXT("MH_EnableHook Error"));
		return FALSE;
	}

	
	if (MH_EnableHook(&TerminateProcess) != MH_OK)
	{
		OutputDebugString(TEXT("MH_EnableHook Error"));
		return FALSE;
	}
**/
	return TRUE;

}


BOOL UnHOOK()
{
	
	
	// Disable the hook.
	if (MH_DisableHook(&CreateProcessA) != MH_OK)
	{
		OutputDebugString(TEXT("MH_DisableHook CreateProcessA Error"));
		return FALSE;
	}
			
	if (MH_DisableHook(&CreateProcessW) != MH_OK)
	{

		OutputDebugString(TEXT("MH_DisableHook CreateProcessW Error"));
		return FALSE;
	}
	
/**	
	if (MH_DisableHook(&DialogBoxIndirectParamA) != MH_OK)
	{
		OutputDebugStringW(TEXT("MH_DisableHook DialogBoxIndirectParamA Error"));
		return FALSE;
	}

	if (MH_DisableHook(&DialogBoxIndirectParamW) != MH_OK)
	{
		OutputDebugStringW(TEXT("MH_DisableHook DialogBoxIndirectParamW Error"));
		return FALSE;
	}

	
	if (MH_DisableHook(&ExitProcess) != MH_OK)
	{
		OutputDebugStringW(TEXT("MH_DisableHook ExitProcess Error"));
		return FALSE;
	}

	
	if (MH_DisableHook(&TerminateProcess) != MH_OK)
	{
		OutputDebugStringW(TEXT("MH_DisableHook TerminateProcess Error"));
		return FALSE;
	}
**/
	// Uninitialize MinHook.
	if (MH_Uninitialize() != MH_OK)
	{
		OutputDebugString(TEXT("MH_Uninitialize Error"));
		return FALSE;
	}

	return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{	
	if (  LefProcess() )
	{		
		return TRUE;
	}
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		LoadNTDLLFunctions();
		HOOK();
		break;

	case DLL_PROCESS_DETACH:
		FreeNTDLLFunctions();
		UnHOOK();		
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;

	}
	return TRUE;
}

/* 
* char* ==> wchar_t*
* ========================================================*/
wchar_t*  MultiByteToWideChar2(const char* pKey)
{
	const char* pCStrKey = pKey;
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t *pWCStrKey = (wchar_t *)malloc(pSize);
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);

	return pWCStrKey;
}
