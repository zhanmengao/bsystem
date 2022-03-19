// VEIDP_NEW.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VEIDP_NEW.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/LogThread.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"UserRoutinue.h"
#include"CollectRoutinue.h"
#define MAX_LOADSTRING 100

unsigned __stdcall CallThreadManager(void* argv)
{
	LogUtils::LogDaily("CallThreadManager");
	while (ThreadManager::Tick())
	{
		Sleep(50);
	}
	return 0;
}
// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	LogUtils::Awake(PathHelper::GetModuleFolder() + "log\\");
	LogUtils::SetTag("VEIDP");
	LogUtils::LogDaily("VEIDP.exe Begin");
	//初始化线程
	ThreadManager::NewThread(new UserRoutinue());
	ThreadManager::NewThread(new CollectRoutinue());
	HANDLE hThm = (HANDLE)_beginthreadex(NULL, NULL, CallThreadManager, NULL, NULL, NULL);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VEIDP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	LogUtils::LogDaily("InitInstance	windows OK");
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEIDP));

	MSG msg = { 0 };
	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	CloseHandle(hThm);
	LogUtils::Destroy();
	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VEIDP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VEIDP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HANDLE hMutext = CreateMutex(NULL, FALSE, "veidp");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		LOG("CreateMutex Error！！！！！");
		//MessageBox(NULL, _T("程序运行中，请不要重复运行！"), _T("信息提示"), MB_OK);
#ifndef _DEBUG
		return 0;
#endif
	}

	hInst = hInstance; // 将实例句柄存储在全局变量中
	int width = 0;
	int high = 0;
	int iShow = SWP_HIDEWINDOW;
#if _DEBUG
	width = 800;
	high = 600;
	iShow = SWP_SHOWWINDOW;
#endif
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		width, 0, width, high, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		LOG("CreateWindowW Error！！！！！");
		return FALSE;
	}
	ShowWindow(hWnd, iShow);
	UpdateWindow(hWnd);
	//设置窗口居中显示
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(hWnd, &rect);
	//重新设置rect里的值
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//移动窗口到指定的位置

	SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, iShow);

	return TRUE;
}



// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
