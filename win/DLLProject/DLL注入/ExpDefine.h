#pragma once
#include<Windows.h>
#include <winternl.h>
#define KERNEL "kernel32.dll"
#define NT "ntdll.dll"
#ifdef _WIN64
typedef DWORD(WINAPI *typedef_ZwCreateThreadEx)(
	PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess,
	LPVOID ObjectAttributes,
	HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	ULONG CreateThreadFlags,
	SIZE_T ZeroBits,
	SIZE_T StackSize,
	SIZE_T MaximumStackSize,
	LPVOID pUnkown);
#else
typedef DWORD(WINAPI *typedef_ZwCreateThreadEx)(
	PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess,
	LPVOID ObjectAttributes,
	HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	BOOL CreateSuspended,
	DWORD dwStackSize,
	DWORD dw1,
	DWORD dw2,
	LPVOID pUnkown);
#endif

typedef NTSTATUS(WINAPI* typedef_NtQueryInformationProcess)(
	HANDLE ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength);

typedef int  (WINAPI *_MessageBoxA)(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType);

typedef int  (WINAPI *_MessageBoxW)(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType);

typedef
BOOL(WINAPI* _WTSSendMessageW)
(
	IN HANDLE hServer,
	IN DWORD SessionId,
	_In_reads_bytes_(TitleLength) LPWSTR pTitle,
	IN DWORD TitleLength,
	_In_reads_bytes_(MessageLength) LPWSTR pMessage,
	IN DWORD MessageLength,
	IN DWORD Style,
	IN DWORD Timeout,
	_Out_ DWORD * pResponse,
	IN BOOL bWait
	);

typedef BOOL(WINAPI* _WTSSendMessageA)
(
	IN HANDLE hServer,
	IN DWORD SessionId,
	_In_reads_bytes_(TitleLength) LPSTR pTitle,
	IN DWORD TitleLength,
	_In_reads_bytes_(MessageLength) LPSTR pMessage,
	IN DWORD MessageLength,
	IN DWORD Style,
	IN DWORD Timeout,
	_Out_ DWORD * pResponse,
	IN BOOL bWait
	);
typedef INT_PTR(WINAPI* _DialogBoxIndirectParamA)(
	_In_opt_ HINSTANCE hInstance,
	_In_ LPCDLGTEMPLATEA hDialogTemplate,
	_In_opt_ HWND hWndParent,
	_In_opt_ DLGPROC lpDialogFunc,
	_In_ LPARAM dwInitParam);

typedef INT_PTR(WINAPI* _DialogBoxIndirectParamW)(
	_In_opt_ HINSTANCE hInstance,
	_In_ LPCDLGTEMPLATEW hDialogTemplate,
	_In_opt_ HWND hWndParent,
	_In_opt_ DLGPROC lpDialogFunc,
	_In_ LPARAM dwInitParam);

typedef
WINADVAPI
BOOL
(WINAPI *_CreateProcessAsUserW)(
	_In_opt_ HANDLE hToken,
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
typedef BOOL(WINAPI *_CreateProcessW)(
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

typedef BOOL(WINAPI *_CreateProcessA)(
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
typedef NTSTATUS(WINAPI* typedef_ZwQuerySystemInformation)(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	);

typedef HMODULE(__stdcall *_LoadLibraryA)(LPCSTR);
typedef FARPROC(__stdcall *_GetProcAddress)(HMODULE, LPCSTR);
typedef HMODULE(__stdcall *_GetModuleHandleA)(LPCSTR);
typedef DWORD(__stdcall *_GetModuleFileNameA)(HMODULE, LPCSTR, DWORD);
struct _InjectDATA
{
	DWORD dwLoadLibrary;
	DWORD dwGetProcAddress;
	DWORD dwGetModuleHandle;
	DWORD dwGetModuleFileName;

	char User32Dll[20] = { 0 };
	char MsgBox[20] = { 0 };
	char text[20] = { 0 };
};
typedef DWORD(WINAPI *RemoteThreadProc)(LPVOID lpParam);