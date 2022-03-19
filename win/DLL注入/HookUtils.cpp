#include"HookUtils.h"
#include"DLLUtils.h"
#pragma comment(lib, "libMinHook.lib")

//高级钩子，需DLL
HHOOK HookUtils::RegMessageHook(HOOKPROC proc, HINSTANCE dllModule)
{
	HHOOK  hMessageHook = SetWindowsHookEx(WH_GETMESSAGE, proc, dllModule, 0);
	return hMessageHook;
}
BOOL HookUtils::DisMessageHook(HHOOK hook)
{
	BOOL bRet = FALSE;
	if (hook)
	{
		bRet = UnhookWindowsHookEx(hook);
		hook = NULL;
	}
	return bRet;
}
MH_STATUS HookUtils::HookFunction(void* pOld, void* pNew, void** pOldFunc)
{
	TCHAR buffer[1024] = { 0 };
	wsprintf(buffer, _T("HookFunction Begin pOldFunc = 0x%p"), *pOldFunc);
	OutputDebugString(buffer);
	MH_STATUS status = MH_Initialize();
	do
	{
		if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED)
		{
			OutputDebugString(TEXT("MH_Initialize Error"));
			break;
		}
		status = MH_CreateHook(pOld, pNew, pOldFunc);
		if (status != MH_OK)
		{
			OutputDebugString(TEXT("MH_CreateHook Error"));
			break;
		}
		status = MH_EnableHook(pOld);
		if (status != MH_OK)
		{
			OutputDebugString(TEXT("MH_EnableHook Error"));
			break;
		}
	} while (0);
	if (status == MH_STATUS::MH_OK)
	{
		TCHAR buffer[1024] = { 0 };
		wsprintf(buffer, _T("HookFunction OK pOldFunc = 0x%p"),*pOldFunc);
		OutputDebugString(buffer);
	}
	else
	{
		wsprintf(buffer, _T("HookFunction Error %d "), status);
		OutputDebugString(buffer);
	}
	return status;
}
MH_STATUS HookUtils::UnHookFunction(void* pOld)
{
	MH_STATUS status = MH_STATUS::MH_OK;
	do
	{
		// Disable the hook.
		status = MH_DisableHook(pOld);
		if (status != MH_OK)
		{
			TCHAR buffer[1024] = { 0 };
			wprintf(buffer, _T("MH_DisableHook Error %d"), status);
			OutputDebugString(buffer);
			break;
		}
	} while (0);
	if (status == MH_STATUS::MH_OK)
	{
		OutputDebugString(_T("UnHookFunction OK "));
	}
	else
	{
		TCHAR buffer[1024] = { 0 };
		wsprintf(buffer, _T("UnHookFunction Error %d "), status);
		OutputDebugString(buffer);
	}
	return status;
}
MH_STATUS HookUtils::UnInitialize()
{
	MH_STATUS status = MH_STATUS::MH_OK;
	do
	{
		// Disable the hook.
		status = MH_Uninitialize();
		if (status != MH_OK)
		{
			TCHAR buffer[1024] = { 0 };
			wprintf(buffer, _T("MH_Uninitialize Error %d"), status);
			OutputDebugString(buffer);
			break;
		}
	} while (0);
	if (status == MH_STATUS::MH_OK)
	{
		OutputDebugString(_T("UnInitialize OK "));
	}
	else
	{
		TCHAR buffer[1024] = { 0 };
		wsprintf(buffer, _T("UnInitialize Error %d "), status);
		OutputDebugString(buffer);
	}
	return status;
}

