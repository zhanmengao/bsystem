#include <windows.h>
#include <iostream>
#include<string>
using namespace std;
HWND mHwnd = NULL;
void InitMouse()
{
	RAWINPUTDEVICE rid;  //设备信息
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02; //rid.usUsagePage = 0x01; rid.usUsage = 0x02; 为鼠标
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = mHwnd;
	bool ret = RegisterRawInputDevices(&rid, 1, sizeof(rid));
	if (ret)
	{
		cout << ("MouseManager RegisterRawInputDevices Sec!") << endl;
	}
	else
	{
		cout << "MouseManager RegisterRawInputDevices Fail! Error = " +
			to_string(GetLastError()) << endl;
	}
}
void Init()
{
	WCHAR pszNewWindowTitle[1024];
	WCHAR pszOldWindowTitle[1024]; 
	GetConsoleTitle(pszOldWindowTitle, 1024);

	// Format a "unique" NewWindowTitle.

	wsprintf(pszNewWindowTitle, L"WBY");
	SetConsoleTitle(pszNewWindowTitle);
	Sleep(40);
	mHwnd = FindWindow(NULL, pszNewWindowTitle);
	SetConsoleTitle(pszOldWindowTitle);
}
void InitClip()
{
	bool ret = AddClipboardFormatListener(mHwnd);
	if (ret)
	{
		cout << "ClipboardManager AddClipboardFormatListener Sec!" << endl;
	}
	else
	{
		cout << "ClipboardManager AddClipboardFormatListener Fail! Error = " +
			to_string(GetLastError()) << endl;
	}
}
void DestClip()
{
	RemoveClipboardFormatListener(mHwnd);
}
void OnClipboardChange();
void ClipMain()
{
	//mHwnd = GetConsoleWindow();
	Init();
	//InitClip();
	InitMouse();
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case WM_CLIPBOARDUPDATE:
			//剪切板事件
			OnClipboardChange();
			break;
		}
	}
	DestClip();
}
void OnClipboardChange()
{
	if (OpenClipboard(NULL))//打开剪贴板  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
		{
			//读取数据  
			HANDLE hClip = GetClipboardData(CF_TEXT);
			char* pBuf = (char*)GlobalLock(hClip);
			cout << string(pBuf) << endl;
			GlobalUnlock(hClip);
			CloseClipboard();
		}
	}
}