#include"stdafx.h"
#include"UserRoutinue.h"
#include "VEIDP_NEW.h"
#include"UserMessage.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"

#include"../../../IPC/ListenerProject/ListenerProject/MessageUtils.h"
#include"CollectMessage.h"
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
		// �����˵�ѡ��: 
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
		//����������һ�βɼ�����
		OnWMEnd(hWnd);
		return 1;
	}
	case WM_CLOSE:
		//ȷ���˳�,���ٴ���,�׳�һ��WM_DESTYRY����Ϣ
		LOG_DAILY("Recv Message WM_CLOSE");
		OnWMEnd(hWnd);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
	{
		LOG_DAILY("Recv Message WM_DESTROY");
		//������ǰ����
		OnWMEnd(hWnd);

		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
//ע��ʱ�ӱ����������������������߳���
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
		//�����ļ�
		conf.ReloadConfig();
		//�����̲߳ɼ�����  ���ö�ʱ��
		int pantTime = conf.pantTime;
		int endTimer = conf.endTick;
		//��ʱ��������
		pantEvent = SetTimer(NULL, PANT_ID, pantTime * 1000 * 60, Tick_Pant);
		//��ʱˢ�¹ػ���Ϣ���뼶��
		endTimeEvent = SetTimer(NULL, END_ID, endTimer * 1000, Tick_End);
		//��¼��־
		char buffer[1024] = { 0 };
		sprintf_s(buffer, _countof(buffer), "Pant: timer %u min , id = %u	End:timer %u s , id = %u ",
			pantTime, pantEvent, endTimer, endTimeEvent);
		LOG_DAILY(buffer);

		//ע���¼�
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
//��Collect����Ϣ
tm LastDrivetm = { 0 };
void OnWMDevice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LOG_DAILY("Recv WM_DEVICECHANGE : " + MessageUtils::GetDeviceNotfiDesc(wParam));
	//�����¼����ɼ�һ��
	if (wParam == DBT_DEVNODES_CHANGED)
	{
		auto msg = MSG_NEW(Msg_EventCollect);
		ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
	}
	else
	{
		//�����λ�ȡ��Ϣ
		tm current = ::GetTime(::GetAnsiTime());
		if (!::CompareSec(current, LastDrivetm, 1))
		{
			//USB�豸�䶯������Ϣ
			auto msg = MSG_NEW(Msg_EventCollect);
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
			LastDrivetm = current;
		}
	}
}

//���ػ�����Ԥ�棬������
void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	auto msg = MSG_NEW(Msg_EndCache);
	ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, msg);
}
void CALLBACK Tick_Pant(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	//������Ϣ��ֱ�ӷ����ɼ��߳�
	auto pMsg = MSG_NEW(Msg_PantCollect);
	ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
}