// VEIDP_NEW.cpp : ����Ӧ�ó������ڵ㡣
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
// ȫ�ֱ���: 
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
	LogUtils::Awake(PathHelper::GetModuleFolder() + "log\\");
	LogUtils::SetTag("VEIDP");
	LogUtils::LogDaily("VEIDP.exe Begin");
	//��ʼ���߳�
	ThreadManager::NewThread(new UserRoutinue());
	ThreadManager::NewThread(new CollectRoutinue());
	HANDLE hThm = (HANDLE)_beginthreadex(NULL, NULL, CallThreadManager, NULL, NULL, NULL);

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VEIDP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	LogUtils::LogDaily("InitInstance	windows OK");
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEIDP));

	MSG msg = { 0 };
	// ����Ϣѭ��: 
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VEIDP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VEIDP);
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
	HANDLE hMutext = CreateMutex(NULL, FALSE, "veidp");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		LOG("CreateMutex Error����������");
		//MessageBox(NULL, _T("���������У��벻Ҫ�ظ����У�"), _T("��Ϣ��ʾ"), MB_OK);
#ifndef _DEBUG
		return 0;
#endif
	}

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
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
		LOG("CreateWindowW Error����������");
		return FALSE;
	}
	ShowWindow(hWnd, iShow);
	UpdateWindow(hWnd);
	//���ô��ھ�����ʾ
	int scrWidth, scrHeight;
	RECT rect;
	//�����Ļ�ߴ�
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//ȡ�ô��ڳߴ�
	GetWindowRect(hWnd, &rect);
	//��������rect���ֵ
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//�ƶ����ڵ�ָ����λ��

	SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, iShow);

	return TRUE;
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
