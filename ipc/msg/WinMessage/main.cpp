#include"WinMessageNotepad.hpp"
#include<iostream>
#include <process.h>
#include"WinMessageBrowser.hpp"
#include<Dbt.h>


MessageNotePad notepad;
void t(void* parameter)
{
	while (1)
	{
		std::cin.clear();
		char c = std::cin.get();
		switch (c)
		{
		case '0':
			notepad.Open(); break;
		case '1':
			notepad.Close(); break;
		case '2':
			notepad.SetTitle("aaa"); break;
		case '3':
			notepad.GetNotepadTitle(); break;
		case '4':
			BrowserStart(); break;
		case '5':
			BrowserEnd(); break;
		case '6':
			SimKeyboard(); break;
		case '7':
			SimMouse(); break;
		}
	}
}

void main()
{
	window.InitWindow(WindowProc);
	DEV_BROADCAST_DEVICEINTERFACE Filter;
	ZeroMemory(&Filter, sizeof(Filter));
	Filter.dbcc_size = sizeof(Filter);
	Filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72, 0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };
	//Filter.dbcc_classguid = { 0xA5DCBF10L, 0x6530, 0x11D2,{ 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	Filter.dbcc_classguid = WceusbshGUID;
	HDEVNOTIFY m_hDeviceNotify = RegisterDeviceNotification(window.GetHWND(), &Filter, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);				//关注所有设备事件  
	_beginthread(t, 0, NULL);
	while (1)
	{
		window.Update();
	}
}