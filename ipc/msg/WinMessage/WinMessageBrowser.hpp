#pragma once
#include<Windows.h>
#include"../../../Windows/WNDUtils/WNDUtils/WindowHelper.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include<atlstr.h>
#include<Dbt.h>
#define BROWSER_TITLE L"�ٶ�һ�£����֪��"
static WindowHelper window;
void __stdcall OnTimer(HWND hwnd, UINT uMsg, UINT_PTR eID, DWORD dwTime)
{
	HWND h = WindowHelper::GetHWNDByClass(BROWSER_TITLE);
	if (h == NULL)
	{
		printf("h is NULL\n");
	}
	PostMessage(h, WM_KEYDOWN, VK_F5, 1);
	Sleep(50);
	PostMessage(h, WM_KEYUP, VK_F5, 1);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		printf("Recv WM_TIMER \n");
		OnTimer(hwnd, message, wParam, lParam);
		break;
	case WM_DEVICECHANGE:
	{
		printf("Recv WM_DEVICECHANGE \n");
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		if (wParam == DBT_DEVICEARRIVAL)
		{
			if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				CString dbcc_name = pDevInf->dbcc_name;
				printf(" %ws �豸�Ѳ���\n", pDevInf->dbcc_name);
				if (dbcc_name.Find(_T("VID_xxxx&PID_xxxx")) != -1)
				{
					MessageBox(NULL, _T("�ض��豸�Ѳ���"), NULL, MB_OK);
				}
			}
		}
		else if (wParam == DBT_DEVICEREMOVECOMPLETE)
		{
			if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				CString dbcc_name = pDevInf->dbcc_name;
				printf(" %ws �豸�Ѱγ�\n", pDevInf->dbcc_name);
				if (dbcc_name.Find(_T("Ven_USBKey&Prod_USBKey")) != -1)
				{
					MessageBox(NULL, _T("�ض��豸�Ѱγ�"), NULL, MB_OK);
				}
			}
		}
		else if (wParam == DBT_DEVNODES_CHANGED)
		{
			printf(" ˢ�ڵ�� ������\n");
		}
	}
	break;
	default:
		return (DefWindowProc(hwnd, message, wParam, lParam));
	}
	return 0;
};
void BrowserStart()
{
	if (SetTimer(window.GetHWND(), 1, 1000, NULL) > 0)
	{
		printf("SetTimer Sec %d \n", GetLastError());
	}
	else
	{
		printf("SetTimer Fail %d \n", GetLastError());
	}
}

void BrowserEnd()
{
	if (KillTimer(window.GetHWND(), 1))
	{
		printf("KillTimer Sec %d \n", GetLastError());
	}
	else
	{
		printf("KillTimer Fail %d \n", GetLastError());
	}
}

void SimKeyboard()
{
	WindowHelper::SetForeground(BROWSER_TITLE);
	Sleep(1000);
	//ģ��F5
	keybd_event(VK_F5, 0, 0, 0);
	Sleep(1000);
	keybd_event(VK_F5, 0, 0, 0);
	Sleep(1000);
	keybd_event(VK_F5, 0, 0, 0);
}

void SimMouse()
{
	WindowHelper::SetForeground(BROWSER_TITLE);
	Sleep(1000);
	HWND hwnd = WindowHelper::GetForegroundHWND();
	POINT pt = { 0 };
	ClientToScreen(hwnd, &pt);
	//ģ������Ҽ�
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(100);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	//�ڵ������Ҽ��˵�����R��
	Sleep(500);
	keybd_event(0x52, 0, 0, 0);
}
