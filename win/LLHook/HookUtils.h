#pragma once
#include<Windows.h>
#include <Winternl.h>

class LLHookUtils
{
public:
	//低级钩子，console可用
	static bool RegKeyboardHook(HOOKPROC proc)
	{
		hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, proc, NULL, 0);
		return hKeyHook != NULL;
	}
	static BOOL DisKeyboardHook()
	{
		BOOL bRet = FALSE;
		if (hKeyHook)
		{
			bRet = UnhookWindowsHookEx(hKeyHook);
			hKeyHook = NULL;
		}
		return bRet;
	}
	static HHOOK GetKeyHook()
	{
		return hKeyHook;
	}
	static bool RegMouseHook(HOOKPROC proc)
	{
		hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, proc, NULL, 0);
		return hMouseHook != NULL;
	}
	static BOOL DisMouseHook()
	{
		BOOL bRet = FALSE;
		if (hMouseHook)
		{
			bRet = UnhookWindowsHookEx(hMouseHook);
			hMouseHook = NULL;
		}
		return bRet;
	}
	static HHOOK GetMouseHook()
	{
		return hMouseHook;
	}
private:
	static HHOOK hKeyHook;
	static HHOOK hMouseHook;
};


HHOOK LLHookUtils::hKeyHook;
HHOOK LLHookUtils::hMouseHook;