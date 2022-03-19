#include"WindowHelper.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
void WindowHelper::InitWindow(WNDPROC proc, HINSTANCE hinstance,
	unsigned x, unsigned y, unsigned w, unsigned h, const TCHAR* className,const TCHAR* title)
{
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.lpfnWndProc = proc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hinstance;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = className;

	if (!RegisterClassEx(&wndclass))
	{
		printf("RegisterClassEx Error %u \n", GetLastError());
	}

	mHwnd = CreateWindowW(className, title, WS_OVERLAPPEDWINDOW,x, y, w, h
		, nullptr, nullptr, hinstance, nullptr);
	//mHwnd = CreateWindowEx(
	//	CW_USEDEFAULT,
	//	wndclass.lpszClassName,
	//	0,
	//	WS_POPUP,
	//	x, y, w, h,
	//	NULL,
	//	NULL,
	//	hinstance,
	//	NULL);
}
void WindowHelper::Show(int nCmdShow)
{
	ShowWindow(mHwnd, nCmdShow);
	UpdateWindow(mHwnd);
}
void WindowHelper::Update()
{
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message != WM_QUIT)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//注销窗口
			UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
		}
	}
}
HWND WindowHelper::GetHWND()
{
	return mHwnd;
}
HINSTANCE WindowHelper::GetHINSTANCE(HWND hwnd)
{
	return  (HINSTANCE)GetWindowLong(hwnd, -6);
}
const TCHAR* WindowHelper::ClassName() const
{
	return wndclass.lpszClassName;
}

HWND WindowHelper::GetHWNDByClass(const stdstr& className)
{
	return FindWindow(className.c_str(), NULL);
}
HWND WindowHelper::GetHWND(const stdstr& winName)
{
	return FindWindow(NULL, winName.c_str());
}
HWND WindowHelper::GetConsoleHWND()
{
	return GetConsoleWindow();
}
stdstr WindowHelper::GetConsoleTITLE()
{
	TCHAR buf[MAX_PATH] = { 0 };
	GetConsoleTitle(buf, MAX_PATH);
	return buf;
}
HWND WindowHelper::GetTopHWND(HWND hwnd)
{
	return GetTopWindow(hwnd);
}
HWND WindowHelper::GetActiveHWND()
{
	return GetActiveWindow();
}
HWND WindowHelper::GetForegroundHWND()
{
	return GetForegroundWindow();
}
void WindowHelper::SetForeground(const TCHAR* title)
{
	SetForeground(GetHWND(title));
}
void WindowHelper::SetForeground(HWND hwnd)
{
	SetForegroundWindow(hwnd);
}
std::wstring WindowHelper::GetWindowTitle(HWND hwnd)
{
	WCHAR buf[1024];
	GetWindowText(hwnd, buf, 1023);
	return buf;
}
DWORD WindowHelper::GetPID(HWND hwnd)
{
	DWORD ldwProID;
	GetWindowThreadProcessId(hwnd, &ldwProID);
	return ldwProID;
}
stdstr WindowHelper::GetProcessName(HWND nlHandle)
{
	//得到该进程的进程id
	DWORD ldwProID = WindowHelper::GetPID(nlHandle);
	return ProcessHelper::GetProcessName(ldwProID);
}

BOOL WindowHelper::SendMsg(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return PostMessage(hwnd, msg, wp, lp);
}
BOOL WindowHelper::KeyEvent(HWND hwnd, USHORT vkCode, bool bSetFore)
{
	if (!bSetFore)
	{
		SendMsg(hwnd, WM_KEYDOWN, vkCode, 1);
		Sleep(50);
		return SendMsg(hwnd, WM_KEYUP, vkCode, 1);
	}
	else
	{
		WindowHelper::SetForeground(hwnd);
		Sleep(1000);
		keybd_event(vkCode, 0, 0, 0);
	}
	return TRUE;
}
BOOL WindowHelper::MouseEvent(HWND hwnd, MEVENT msg, POINT pt, bool bSetFore)
{
	if (!bSetFore)
	{

	}
	else
	{
		WindowHelper::SetForeground(hwnd);
		Sleep(1000);
		if (msg == MEVENT::RIGHT)
		{
			mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, 0);
			Sleep(100);
			mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, 0);
		}
		else if (msg == MEVENT::LEFT)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, 0);
			Sleep(100);
			mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0);
		}

	}
	return TRUE;
}
POINT WindowHelper::GetMousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	return pt;
}
BOOL WindowHelper::SetMousePos(POINT pt)
{
	return SetCursorPos(pt.x, pt.y);
}
BOOL WindowHelper::SetMousePos(HWND hwnd, POINT pt)
{
	GetClientPoint(hwnd, pt);
	return SetMousePos(pt);
}
POINT WindowHelper::GetMousePos(HWND hwnd)
{
	auto pt = GetMousePos();
	GetClientPoint(hwnd, pt);
	return pt;
}
void WindowHelper::LitmitCursor(const HWND &hwnd)
{
	RECT rect;
	POINT lt, rb;
	//拿到窗口内部的矩形
	GetClientRect(hwnd, &rect);

	lt.x = rect.left;
	lt.y = rect.top;

	rb.x = rect.right;
	rb.y = rect.bottom;
	//转化两个点为屏幕坐标
	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	ClipCursor(&rect);
}
BOOL WindowHelper::GetClientPoint(HWND hwnd, POINT& pt)
{
	return ScreenToClient(hwnd, &pt);
}
BOOL WindowHelper::GetScreenPoint(HWND hwnd, POINT& pt)
{
	return ClientToScreen(hwnd, &pt);
}
int WindowHelper::GetViewWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}
int WindowHelper::GetViewHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}
RECT WindowHelper::GetWindowSize(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	return rect;
}