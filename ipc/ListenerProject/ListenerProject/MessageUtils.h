#pragma once
#include<Windows.h>
#include<Dbt.h>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include<tchar.h>

class MessageUtils
{
public:
	//注册
	static HDEVNOTIFY RegDeviceNotification(HWND hwnd)
	{
		DEV_BROADCAST_DEVICEINTERFACE Filter;
		ZeroMemory(&Filter, sizeof(Filter));
		Filter.dbcc_size = sizeof(Filter);
		Filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72, 0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };
		//Filter.dbcc_classguid = { 0xA5DCBF10L, 0x6530, 0x11D2,{ 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
		Filter.dbcc_classguid = WceusbshGUID;
		//DEVICE_NOTIFY_ALL_INTERFACE_CLASSES //关注所有设备事件  
		return RegisterDeviceNotification(hwnd, &Filter, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);
	}
	static BOOL RegRawKeyboard(HWND hwnd)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x06;
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.hwndTarget = hwnd;
		return RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
	}
	static BOOL RegRawMouse(HWND hwnd)
	{
		RAWINPUTDEVICE rid;  //设备信息
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02; //rid.usUsagePage = 0x01; rid.usUsage = 0x02; 为鼠标
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.hwndTarget = hwnd;
		return RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
	}
public:
	//解析
	static stdstr GetDeviceNotifiData(DWORD dwData, PDEV_BROADCAST_HDR pHdr,PDEV_BROADCAST_DEVICEINTERFACE pData)
	{
		pHdr = (PDEV_BROADCAST_HDR)dwData;
		pData = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
		return GetDeviceNotfiDesc(pHdr->dbch_devicetype);
	}
	static stdstr GetDeviceNotfiDesc(UINT nEventType)
	{
		switch (nEventType)
		{
		case DBT_CONFIGCHANGECANCELED:
			return _T("DBT_CONFIGCHANGECANCELED");
			break;
		case DBT_CONFIGCHANGED:
			return _T("DBT_CONFIGCHANGED");
			break;

		case DBT_CUSTOMEVENT:
			return _T("DBT_CUSTOMEVENT");

		case DBT_DEVICEARRIVAL:
			return _T("DBT_DEVICEARRIVAL");
			break;
		case DBT_DEVICEQUERYREMOVE:
			return _T("DBT_DEVICEQUERYREMOVE");
			break;

		case DBT_DEVICEQUERYREMOVEFAILED:
			return _T("DBT_DEVICEQUERYREMOVEFAILED");
			break;

		case DBT_DEVICEREMOVECOMPLETE:
			return _T("DBT_DEVICEREMOVECOMPLETE");
			break;

		case DBT_DEVICEREMOVEPENDING:
			return _T("DBT_DEVICEREMOVEPENDING");
			break;

		case DBT_DEVICETYPESPECIFIC:
			return _T("DBT_DEVICETYPESPECIFIC");
			break;

		case DBT_DEVNODES_CHANGED:
			return _T("DBT_DEVNODES_CHANGED");
			break;

		case DBT_QUERYCHANGECONFIG:
			return _T("DBT_QUERYCHANGECONFIG");
			break;

		case DBT_USERDEFINED:
			return _T("DBT_USERDEFINED");
			break;
		default:
			return _T("default");
		}
	}

	static MSG GetDeviceInput(const MSG& msg)
	{
		MSG rMsgCpy = { 0 };
		LPBYTE lpb = NULL;
		do
		{
			//解析消息
			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL,
				&dwSize, sizeof(RAWINPUTHEADER)); //先获取数据大小dwSize
			LPBYTE lpb = new BYTE[dwSize]; //根据大小 创建

			if (GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb,
				&dwSize, sizeof(RAWINPUTHEADER)) != dwSize) //获取消息信息
				break;
			RAWINPUT *raw = (RAWINPUT*)lpb;
			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				if (raw->data.keyboard.Message == WM_KEYDOWN)
				{
					//把事件做转换
					rMsgCpy.wParam = raw->data.keyboard.VKey;  //按键 键值;
					rMsgCpy.message = WM_KEYDOWN;
				}
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				if (raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
				{
					rMsgCpy.message = WM_LBUTTONDOWN;
				}
				else if (raw->data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)
				{
					rMsgCpy.message = WM_RBUTTONDOWN;
				}
			}
		} while (false);
		delete[] lpb;
		return rMsgCpy;
	}
};