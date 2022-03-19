// VeidpConfig.cpp : ����Ӧ�ó������ڵ㡣
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

// ȫ�ֱ���: 
Config mConfig;
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VEIDPCONFIG, szWindowClass, MAX_LOADSTRING);

	//�����ﴦ��
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

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEIDPCONFIG));

	MSG msg;

	// ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
		CreateWindow(L"static", _T("���͵�IP..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hSendIP = CreateWindow(L"edit", mConfig.commonInfo.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("���͵��˿�..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hSendPort = CreateWindow(L"edit", mConfig.commonInfo.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("���ͻ���"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hSendTopic = CreateWindow(L"edit", mConfig.commonInfo.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("��IP����..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hRecvIP = CreateWindow(L"edit", mConfig.instructInfo.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("��IP�ĸö˿ڽ���..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hRecvPort = CreateWindow(L"edit", mConfig.instructInfo.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("���ջ���"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hRecvTopic = CreateWindow(L"edit", mConfig.instructInfo.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("��������IP..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hWanSendIP = CreateWindow(L"edit", mConfig.wanProduce.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("�������Ͷ˿�..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hWanSendPort = CreateWindow(L"edit", mConfig.wanProduce.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("�������ͻ���"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hWanSendTopic = CreateWindow(L"edit", mConfig.wanProduce.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("��������IP..."),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_IP_LABEL, hInst, NULL);
		hWanRecvIP = CreateWindow(L"edit", mConfig.wanConsu.IP.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_IP_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("�������ն˿�"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hWanRecvPort = CreateWindow(L"edit", mConfig.wanConsu.Port.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)SEND_PORT_INPUT, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("�������ջ���"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_LABEL, hInst, NULL);
		hWanRecvTopic = CreateWindow(L"edit", mConfig.wanConsu.Topic.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)SEND_TOPIC_INPUT, hInst, NULL);

		high += 50;
		CreateWindow(L"static", _T("������������ӣ�"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hTimer = CreateWindow(L"edit", TOSTR(mConfig.pantTime).c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
			180, high, 160, 18, hWnd, (HMENU)INPUT_TIMER, hInst, NULL);

		high += 30;
		CreateWindow(L"static", _T("��֯����"),
			SS_SIMPLE | WS_CHILD | WS_VISIBLE,
			50, high, 160, 18, hWnd, (HMENU)SEND_PORT_LABEL, hInst, NULL);
		hOrg = CreateWindow(L"edit", mConfig.orgCode.c_str(),
			ES_LEFT | WS_BORDER | WS_CHILD | WS_VISIBLE,
			180, high, 160, 18, hWnd, (HMENU)INPUT_ORG, hInst, NULL);

		high += 60;
		CreateWindow(L"button", L"ȷ��", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			100, high, 50, 42, hWnd, (HMENU)BTN_ENTER, hInst, NULL);
		CreateWindow(L"button", L"�ر�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			240, high, 50, 42, hWnd, (HMENU)BTN_CANCEL, hInst, NULL);
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case BTN_ENTER:
		{
			//д�����ļ�

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
			//ֱ���˳�
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
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		//���нű�
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("stop.bat"), _T(""),true);
		//�˳���ֱ�����������������
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("smve.exe"), _T(""));
		PostQuitMessage(0);
	}

	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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
