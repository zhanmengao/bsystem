#include"HookUtils.h"
#include<stdio.h>
#include"LogManager.hpp"
#include"../DLL×¢Èë/DLLUtils.h"
#include"PathHelper.hpp"
extern "C" __declspec(dllexport) VOID DllRegisterServer();
LRESULT CALLBACK LLHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
		{
			static char key_value[32] = { 0 };
			KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			sprintf_s(key_value, "KeyboardProc nCode = %c", kbdStruct.vkCode);
			printf(key_value);
		}
		break;
		case WM_RBUTTONDOWN:
		{
			printf("WM_RBUTTONDOWN");
		}
		break;
		}
	}
	return CallNextHookEx(LLHookUtils::GetKeyHook(), nCode, wParam, lParam);
}

void mainLLHook()
{
	MessageBox(NULL, L"QQQ", L"AAA", MB_OK);
	if (LLHookUtils::RegKeyboardHook(LLHookProc))
	{
		printf("RegKeyboardHook Sec");
	}
	else
	{
		char buf[1024] = { 0 };
		sprintf(buf, "RegKeyboardHook Error %u", GetLastError());
		printf(buf);
	}
	if (LLHookUtils::RegMouseHook(LLHookProc))
	{
		printf("RegMouseHook Sec");
	}
	else
	{
		char buf[1024] = { 0 };
		sprintf(buf, "RegMouseHook Error %u", GetLastError());
		printf(buf);
	}	MessageBox(NULL, L"QQQ", L"AAA", MB_OK);
	if (LLHookUtils::RegKeyboardHook(LLHookProc))
	{
		printf("RegKeyboardHook Sec");
	}
	else
	{
		char buf[1024] = { 0 };
		sprintf(buf, "RegKeyboardHook Error %u", GetLastError());
		printf(buf);
	}
	if (LLHookUtils::RegMouseHook(LLHookProc))
	{
		printf("RegMouseHook Sec");
	}
	else
	{
		char buf[1024] = { 0 };
		sprintf(buf, "RegMouseHook Error %u", GetLastError());
		printf(buf);
	}
	LLHookUtils::DisKeyboardHook();
	LLHookUtils::DisMouseHook();
}