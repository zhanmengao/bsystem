#pragma once
#include<Windows.h>
#include<string>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include<tchar.h>
class WindowHelper
{
public:
	enum MEVENT
	{
		LEFT = 0,
		RIGHT = 1,
	};
public:
	void InitWindow(WNDPROC proc, HINSTANCE hinstance = NULL,
		unsigned x = 0, unsigned y = 0, unsigned w = 0, unsigned h = 0,
		const TCHAR* className = L"WINCLASS",const TCHAR* title = _T("*"));
	void Show(int nCmdShow);
	void Update();
	HWND GetHWND();
	const TCHAR* ClassName() const;
public:
	static stdstr GetProcessName(HWND nlHandle);
	static HWND GetHWNDByClass(const stdstr& className);
	static HWND GetHWND(const stdstr& winName);
	static HINSTANCE GetHINSTANCE(HWND hwnd);
	static HWND GetConsoleHWND();
	static stdstr GetConsoleTITLE();
	static HWND GetTopHWND(HWND hwnd = NULL);
	static HWND GetActiveHWND();
	static stdstr GetWindowTitle(HWND hwnd);
	static DWORD GetPID(HWND hwnd);
	static HWND GetForegroundHWND();
public:
	static void SetForeground(HWND hwnd);
	static void SetForeground(const TCHAR* title);

	static int GetViewWidth();
	static int GetViewHeight();
	static RECT GetWindowSize(HWND hWnd);
public:
	static BOOL SendMsg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static BOOL KeyEvent(HWND hwnd, USHORT vkCode, bool bSetFore = false);
	static BOOL MouseEvent(HWND hwnd, MEVENT msg, POINT pt, bool bSetFore = false);
	static BOOL SetMousePos(POINT pt);
	static BOOL SetMousePos(HWND hwnd, POINT pt);
	static POINT GetMousePos(HWND hwnd);
	static POINT GetMousePos();
	static void LitmitCursor(const HWND &hwnd);
public:
	static BOOL GetClientPoint(HWND hwnd, POINT& pt);
	static BOOL GetScreenPoint(HWND hwnd, POINT& pt);
private:
	HWND mHwnd;
	WNDCLASSEX wndclass = { 0 };

	MSG msg = { 0 };

};