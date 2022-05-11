// VeidpConfig.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VeidpConfig.h"
#include"../../../../System/Setting/Readini/INIManager.hpp"
#include"../../../../C++space/StringTools/StringTools/StrUtils.h"
#include"../../../../System/File/FileManager/FileManager/PathHelper.hpp"
#include"../../../../System/Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../../../System/Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"ConfigManager.h"

#define MAX_LOADSTRING 100

// 全局变量: 
Config mConfig;
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

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VEIDPCONFIG, szWindowClass, MAX_LOADSTRING);

	//在这里处理
	iniManager ini(PathHelper::GetModuleFolder() + _T("veidp.ini"));
	bool bConfig = ini.ReadNum(_T("begin"), _T("config"));
	if (!bConfig)
	{
		mConfig.ReadConfig();
		mConfig.SaveConfig();
		mConfig.Destroy();
		return FALSE;
	}

	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEIDPCONFIG));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VEIDPCONFIG));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VEIDPCONFIG);
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
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		700, 100, 400, 800, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
HWND hSendIP = NULL;
HWND hSendPort = NULL;
HWND hSendTopic = NULL;
HWND hRecvIP = NULL;
HWND hRecvPort = NULL;
HWND hRecvTopic = NULL;

HWND hWanSendIP = NULL;
HWND hWanSendPort = NULL;
HWND hWanSendTopic = NULL;
HWND hWanRecvIP = NULL;
HWND hWanRecvPort = NULL;
HWND hWanRecvTopic = NULL;

HWND hTimer = NULL;
HWND hOrg = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		int high = 30;
		mConfig.ReadConfig();
		CreateWindow(L"static", _T("发送到IP..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hSendIP = CreateWindow(L"edit", mConfig.commonInfo.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("发送到端口..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hSendPort = CreateWindow(L"edit", mConfig.commonInfo.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("发送话题"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hSendTopic = CreateWindow(L"edit", mConfig.commonInfo.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("从IP接收..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hRecvIP = CreateWindow(L"edit", mConfig.instructInfo.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("从IP的该端口接收..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hRecvPort = CreateWindow(L"edit", mConfig.instructInfo.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("接收话题"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hRecvTopic = CreateWindow(L"edit", mConfig.instructInfo.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("外网发送IP..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hWanSendIP = CreateWindow(L"edit", mConfig.wanProduce.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("外网发送端口..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hWanSendPort = CreateWindow(L"edit", mConfig.wanProduce.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("外网发送话题"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hWanSendTopic = CreateWindow(L"edit", mConfig.wanProduce.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("外网接收IP..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hWanRecvIP = CreateWindow(L"edit", mConfig.wanConsu.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("外网接收端口"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hWanRecvPort = CreateWindow(L"edit", mConfig.wanConsu.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("外网接收话题"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hWanRecvTopic = CreateWindow(L"edit", mConfig.wanConsu.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("心跳间隔（分钟）"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hTimer = CreateWindow(L"edit", TOSTR(mConfig.pantTime).c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)INPUT_TIMER, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("组织名称"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hOrg = CreateWindow(L"edit", mConfig.orgCode.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)INPUT_ORG, hInst, NULL);

		high += 60;
		CreateWindow(L"button", L"确认", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			100, high, 50, 42, hWnd, (HMENU)BTN_ENTER, hInst, NULL);
		CreateWindow(L"button", L"关闭", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			240, high, 50, 42, hWnd, (HMENU)BTN_CANCEL, hInst, NULL);
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case BTN_ENTER:
		{
			//写配置文件

			TCHAR buf[256] = { 0 };
			TCHAR portbuf[256] = { 0 };
			{
				SendMessage(hSendIP, WM_GETTEXT, 256, (LPARAM)buf);
				SendMessage(hSendPort, WM_GETTEXT, 256, (LPARAM)portbuf);
				mConfig.commonInfo.IP = buf;
				mConfig.commonInfo.Port = portbuf;

				SendMessage(hSendTopic, WM_GETTEXT, 256, (LPARAM)buf);
				mConfig.commonInfo.Topic = buf;
			}

			{
				SendMessage(hRecvIP, WM_GETTEXT, 256, (LPARAM)buf);
				SendMessage(hRecvPort, WM_GETTEXT, 256, (LPARAM)portbuf);
				mConfig.instructInfo.IP = buf;
				mConfig.instructInfo.Port = portbuf;

				SendMessage(hRecvTopic, WM_GETTEXT, 256, (LPARAM)buf);
				mConfig.instructInfo.Topic = buf;
			}


			{
				SendMessage(hWanSendIP, WM_GETTEXT, 256, (LPARAM)buf);
				SendMessage(hWanSendPort, WM_GETTEXT, 256, (LPARAM)portbuf);
				mConfig.wanProduce.IP = buf;
				mConfig.wanProduce.Port = portbuf;

				SendMessage(hWanSendTopic, WM_GETTEXT, 256, (LPARAM)buf);
				mConfig.wanProduce.Topic = buf;
			}

			{
				SendMessage(hWanRecvIP, WM_GETTEXT, 256, (LPARAM)buf);
				SendMessage(hWanRecvPort, WM_GETTEXT, 256, (LPARAM)portbuf);
				mConfig.wanConsu.IP = buf;
				mConfig.wanConsu.Port = portbuf;

				SendMessage(hWanRecvTopic, WM_GETTEXT, 256, (LPARAM)buf);
				mConfig.wanConsu.Topic = buf;
			}
			SendMessage(hTimer, WM_GETTEXT, 256, (LPARAM)buf);
			

			SendMessage(hOrg, WM_GETTEXT, 256, (LPARAM)buf);
			mConfig.SaveConfig();

			DestroyWindow(hWnd);
		}
		break;
		case BTN_CANCEL:
		{
			//直接退出
			DestroyWindow(hWnd);
		}
		break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		//运行脚本
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("stop.bat"), _T(""),true);
		//退出后直接启动创建服务进程
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("smve.exe"), _T(""));
		PostQuitMessage(0);
	}

	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
