// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include<Windows.h>
#include"../DLLע��/HookUtils.h"
#include"../DLLע��/ExpDefine.h"

extern HMODULE g_hModule;
#define EXPORT extern "C" __declspec(dllexport)
#pragma data_seg("mydata")
HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:mydata,RWS")

EXPORT HHOOK SetHook();
EXPORT BOOL UnsetHook(HHOOK hHook);
HMODULE g_hModule;
// ��Ϣȫ�ֹ��ӻص�����
LRESULT CALLBACK GetMsgProc(
	int code,       // hook code
	WPARAM wParam,  // removal option  
	LPARAM lParam   // message
)
{
	// �������κβ���������ȫ�ֹ��ӵ�Ŀ�ľ��ǽ���DLLע����ѣ���Ҫ������ڽ��е�API�ҹ�
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}
_MessageBoxW oldMsgBoxW;						//���վɵĺ������
_MessageBoxA oldMsgBoxA;						//���վɵĺ������
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
	if (wcscmp(lpCaption, L"���м�ʱ���ѹ���") == 0)
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
	if (strcmp(lpCaption, "���м�ʱ���ѹ���") == 0)
	{
		return FALSE;
	}
	return oldMsgBoxA(hWnd, lpText, lpCaption, uType);
}
// ����ȫ�ֹ���
HHOOK SetHook()
{
	// ����ȫ�ֹ���
	HHOOK hHook = HookUtils::RegMessageHook(GetMsgProc, g_hModule);
	if (NULL == hHook)
	{
		return NULL;
	}
	g_hHook = hHook;
	return hHook;
}


// ж��ȫ�ֹ���
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


