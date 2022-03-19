// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include<Windows.h>
#include"../DLLע��/HookUtils.h"
#include"../DLLע��/ExpDefine.h"
#include<WtsApi32.h>

#define EXPORT extern "C" __declspec(dllexport)
#pragma comment(lib, "Wtsapi32.lib")
extern HMODULE g_hModule;

#pragma data_seg("mydata")
HHOOK g_hHook = NULL;
#pragma data_seg()

#pragma comment(linker, "/SECTION:mydata,RWS")

EXPORT HHOOK SetHook();
EXPORT BOOL UnsetHook(HHOOK hHook);
HMODULE g_hModule;


LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	// �������κβ���������ȫ�ֹ��ӵ�Ŀ�ľ��ǽ���DLLע����ѣ���Ҫ������ڽ��е�API�ҹ�
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}
_WTSSendMessageW oldWtsMsgW = NULL;					//���վɵĺ������
_WTSSendMessageA oldWtsMsgA = NULL;					//���վɵĺ������
BOOL WINAPI MyWTSSendMessageW(
	IN HANDLE hServer,
	IN DWORD SessionId,
	_In_reads_bytes_(TitleLength) LPWSTR pTitle,
	IN DWORD TitleLength,
	_In_reads_bytes_(MessageLength) LPWSTR pMessage,
	IN DWORD MessageLength,
	IN DWORD Style,
	IN DWORD Timeout,
	_Out_ DWORD * pResponse,
	IN BOOL bWait)
{
	OutputDebugStringA("MyWTSSendMessageW Call");
	WCHAR buffer[1024] = { 0 };
	wsprintfW(buffer, L"MyWTSSendMessageW title = %s message = %s", pTitle, pMessage);
	OutputDebugStringW(buffer);
	if (wcscmp(pTitle, L"���м�ʱ���ѹ���") == 0)
	{
		return FALSE;
	}
	return oldWtsMsgW(hServer,
		SessionId, pTitle, TitleLength, pMessage, MessageLength, Style, Timeout, pResponse, bWait);
}

BOOL WINAPI MyWTSSendMessageA(
	IN HANDLE hServer,
	IN DWORD SessionId,
	_In_reads_bytes_(TitleLength) LPSTR pTitle,
	IN DWORD TitleLength,
	_In_reads_bytes_(MessageLength) LPSTR pMessage,
	IN DWORD MessageLength,
	IN DWORD Style,
	IN DWORD Timeout,
	_Out_ DWORD * pResponse,
	IN BOOL bWait)
{
	OutputDebugStringA("MyWTSSendMessageA Call");
	char buffer[1024] = { 0 };
	wsprintfA(buffer, "MyWTSSendMessageA title = %s message = %s", pTitle, pMessage);
	OutputDebugStringA(buffer);
	if (strcmp(pTitle, "���м�ʱ���ѹ���") == 0)
	{
		return FALSE;
	}
	return oldWtsMsgA(hServer,
		SessionId, pTitle, TitleLength, pMessage, MessageLength, Style, Timeout, pResponse, bWait);
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
	if (!hHook)
	{
		OutputDebugStringA("UnsetHook Error��HOOK Is Null");
		return FALSE;
	}
	return HookUtils::DisMessageHook(hHook);
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hModule = hModule;
		MH_STATUS status = MH_Initialize();
		{
			status = MH_CreateHook(&WTSSendMessageW, &MyWTSSendMessageW,
				reinterpret_cast<void**>(&oldWtsMsgW));
			if (status != MH_OK)
			{
				OutputDebugString(TEXT("WTSSendMessageW MH_CreateHook Error"));
			}
			status = MH_EnableHook(&WTSSendMessageW);
			if (status != MH_OK)
			{
				OutputDebugString(TEXT("WTSSendMessageW MH_EnableHook Error"));
			}
		}
		{
			status = MH_CreateHook(&WTSSendMessageA, &MyWTSSendMessageA,
				reinterpret_cast<void**>(&oldWtsMsgA));
			if (status != MH_OK)
			{
				OutputDebugString(TEXT("WTSSendMessageA MH_CreateHook Error"));
			}
			status = MH_EnableHook(&WTSSendMessageA);
			if (status != MH_OK)
			{
				OutputDebugString(TEXT("WTSSendMessageA MH_EnableHook Error"));
			}
		}
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
		HookUtils::UnHookFunction(&WTSSendMessageW);
		HookUtils::UnHookFunction(&WTSSendMessageA);
		HookUtils::UnInitialize();
		break;
	}
	default:
		break;
	}
	return TRUE;
}


