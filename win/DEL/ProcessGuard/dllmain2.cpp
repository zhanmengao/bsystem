// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include "MinHook.h"
#include <stdio.h>

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

typedef int (WINAPI *CREATEPROCESSW)(_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation);

CREATEPROCESSW pfnCreateProcessW = NULL;

int WINAPI MyCreateProcessW(_In_opt_ LPCWSTR lpApplicationName,
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

	DWORD addr = ((DWORD)lpStartupInfo) - 0x60;

	LPCWSTR lpProcessDirectory = LPCWSTR(*((DWORD *)addr));

	OutputDebugStringW(lpProcessDirectory);

	int nRetn = pfnCreateProcessW(lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		lpProcessInformation);

	wchar_t buf[256] = { 0 };
	swprintf_s(buf, 20, L"Hook CreateProcessPID: %d", lpProcessInformation->dwProcessId);
	OutputDebugStringW(buf);

	return nRetn;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	if (MH_Initialize() == MH_OK)
	{
		OutputDebugStringW(L"MH_Initialize success");
	}

	if (MH_CreateHook(&CreateProcessW, &MyCreateProcessW, reinterpret_cast<void **>(&pfnCreateProcessW)) == MH_OK)
	{
		OutputDebugStringW(L"MH_CreateHook success");
	}

	if (MH_EnableHook(&CreateProcessW) == MH_OK)
	{
		OutputDebugStringW(L"MH_Initialize success");
	}

	return TRUE;
}
