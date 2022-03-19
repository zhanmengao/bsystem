#include"stdafx.h"
#include"UserRoutinue.h"
#include "VEIDP_NEW.h"
#include"UserMessage.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"

#include"../../../IPC/ListenerProject/ListenerProject/MessageUtils.h"
#include"CollectMessage.h"
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		LOG_DAILY("Recv WM_CREATE");
		OnWMCreate(hWnd);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_COMM_1:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_COLLALL;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
		case ID_COMM_2:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_SCREEN;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
			break;
		case ID_COMM_3:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_DISK;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
		case ID_COMM_4:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_MEMORY;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
		case ID_COMM_5:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_MONIT;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}

		case ID_COMM_6:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_EVENT;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
		case ID_COMM_7:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_USB;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
		case ID_COMM_8:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_START;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
			break;
		case ID_COMM_9:
		{
			auto pMsg = MSG_NEW(Msg_CommandCollect);
			pMsg->command = COMMAND::COMM_HARDWARE;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			break;
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DEVICECHANGE:
	{
		OnWMDevice(hWnd, wParam, lParam);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_QUERYENDSESSION:
	{
		LOG_DAILY("Recv Message WM_QUERYENDSESSION");
		//结束程序发送一次采集数据
		OnWMEnd(hWnd);
		return 1;
	}
	case WM_CLOSE:
		//确定退出,销毁窗口,抛出一个WM_DESTYRY的消息
		LOG_DAILY("Recv Message WM_CLOSE");
		OnWMEnd(hWnd);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
	{
		LOG_DAILY("Recv Message WM_DESTROY");
		//结束当前进程
		OnWMEnd(hWnd);

		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
//注册时钟必须放在这里做，处理放在线程里
UINT_PTR pantEvent = 0;
UINT_PTR endTimeEvent = 0;
#define PANT_ID 10
#define END_ID 11
bool bEnd = false;
void OnWMCreate(HWND hWnd)
{
	try
	{
		Config conf;
		//配置文件
		conf.ReloadConfig();
		//心跳线程采集数据  设置定时器
		int pantTime = conf.pantTime;
		int endTimer = conf.endTick;
		//定时发送心跳
		pantEvent = SetTimer(NULL, PANT_ID, pantTime * 1000 * 60, Tick_Pant);
		//定时刷新关机信息（秒级）
		endTimeEvent = SetTimer(NULL, END_ID, endTimer * 1000, Tick_End);
		//记录日志
		char buffer[1024] = { 0 };
		sprintf_s(buffer, _countof(buffer), "Pant: timer %u min , id = %u	End:timer %u s , id = %u ",
			pantTime, pantEvent, endTimer, endTimeEvent);
		LOG_DAILY(buffer);

		//注册事件
		auto pHandle = MessageUtils::RegDeviceNotification(hWnd);
		if (pHandle)
		{
			LOG_DAILY("RegDeviceNotification Sec");
		}
		else
		{
			LOG("RegDeviceNotification Fail");
		}

		auto msg = MSG_NEW(Msg_Create);
		msg->hWnd = hWnd;
		ThreadManager::PushMsg(THREAD_ID::THREAD_USER, msg);
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void OnWMEnd(HWND hWnd)
{

	try
	{
		if (!bEnd)
		{
			bEnd = true;
			LOG_DAILY("OnWMEnd Begin");
			BOOL bRet = KillTimer(hWnd, PANT_ID);
			DWORD dwErr = GetLastError();
			bRet = KillTimer(hWnd, END_ID);
			dwErr = GetLastError();
			auto msg = MSG_NEW(Msg_Exit);
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
			ThreadManager::Destroy();
			LOG_DAILY("OnWMEnd End");
		}
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
//给Collect发消息
tm LastDrivetm = { 0 };
void OnWMDevice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LOG_DAILY("Recv WM_DEVICECHANGE : " + MessageUtils::GetDeviceNotfiDesc(wParam));
	//发生事件，采集一下
	if (wParam == DBT_DEVNODES_CHANGED)
	{
		auto msg = MSG_NEW(Msg_EventCollect);
		ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
	}
	else
	{
		//避免多次获取消息
		tm current = ::GetTime(::GetAnsiTime());
		if (!::CompareSec(current, LastDrivetm, 1))
		{
			//USB设备变动，发消息
			auto msg = MSG_NEW(Msg_EventCollect);
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
			LastDrivetm = current;
		}
	}
}

//将关机数据预存，但不发
void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	auto msg = MSG_NEW(Msg_EndCache);
	ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
}
void CALLBACK Tick_Pant(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	//心跳消息，直接发给采集线程
	auto pMsg = MSG_NEW(Msg_PantCollect);
	ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
}