#include <windows.h>
#include <iostream>
#include<string>
using namespace std;
HWND mHwnd = NULL;
void InitMouse()
{
	RAWINPUTDEVICE rid;  //�豸��Ϣ
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02; //rid.usUsagePage = 0x01; rid.usUsage = 0x02; Ϊ���
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
			//���а��¼�
			OnClipboardChange();
			break;
		}
	}
	DestClip();
}
void OnClipboardChange()
{
	if (OpenClipboard(NULL))//�򿪼�����  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//�жϸ�ʽ�Ƿ�����������Ҫ  
		{
			//��ȡ����  
			HANDLE hClip = GetClipboardData(CF_TEXT);
			char* pBuf = (char*)GlobalLock(hClip);
			cout << string(pBuf) << endl;
			GlobalUnlock(hClip);
			CloseClipboard();
		}
	}
}