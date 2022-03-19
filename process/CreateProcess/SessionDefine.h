#pragma once
#include<Windows.h>
#include <WtsApi32.h>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include<tchar.h>

typedef struct _RDPLoginUserInfo
{
	DWORD SessionId;             // session id

	stdstr  DeviceId;

	stdstr  WTSInitialProgram; //��ʼӦ�ó�����
	stdstr  WTSApplicationName; //Ӧ������
	stdstr  WTSWorkingDirectory; //����Ŀ¼�����������ӵĳ���

	stdstr  WTSUserName;  //�û���
	stdstr  WTSWinStationName; //������
	stdstr  WTSDomainName;  //����

	stdstr  WTSClientName;  //�ͻ�������
	stdstr  WTSClientAddress; //�ͻ���IP ��ַ
	stdstr  WTSClientDirectory; //


	ULONG ClientBuildNumber;
	ULONG ClientHardwareId;
	USHORT ClientProductId;

	BYTE EncryptionLevel;       // ����PD�İ�ȫ����

	USHORT HRes;
	USHORT VRes;
	USHORT ColorDepth;

	void SClear()
	{
		SessionId = 0;

		EncryptionLevel = 0;

		ClientBuildNumber = 0;
		ClientHardwareId = 0;
		ClientProductId = 0;

		HRes = 0;
		VRes = 0;
		ColorDepth = 0;
	}
}RDPLoginUserInfo, *PRDPLoginUserInfo;
typedef struct _RDPLoginSessionInfo
{
	DWORD sid;
	stdstr sName;
	WTS_CONNECTSTATE_CLASS state;
	_RDPLoginUserInfo User;
public:
	stdstr GetStateDesc()
	{
		stdstr State;
		switch (state)
		{
		case WTSActive:              // User logged on to WinStation
			State = _T("WTSActive");
			break;
		case WTSConnected:           // WinStation connected to client
			State = _T("WTSConnected");
			break;
		case WTSConnectQuery:        // In the process of connecting to client
			State = _T("WTSConnectQuery");
			break;
		case WTSShadow:              // Shadowing another WinStation
			State = _T("WTSShadow");
			break;
		case WTSDisconnected:        // WinStation logged on without client
			State = _T("WTSDisconnected");
			break;
		case WTSIdle:                // Waiting for client to connect
			State = _T("WTSIdle");
			break;
		case WTSListen:              // WinStation is listening for connection
			State = _T("WTSListen");
			break;
		case WTSReset:               // WinStation is being reset
			State = _T("WTSReset");
			break;
		case WTSDown:                // WinStation is down due to error
			State = _T("WTSConnected");
			break;
		case WTSInit:                // WinStation in initialization	
			State = _T("WTSInit");
			break;
		default:
			break;
		}
		return State;
	}
}RDPLoginSessionInfo, *PRDPLoginSessionInfo;
class WTSProcessInfo
{
public:
	DWORD SessionId;
	DWORD ProcessId;
	stdstr pProcessName;
	DWORD NumberOfThreads;
	DWORD HandleCount;
	DWORD PagefileUsage;
	DWORD PeakPagefileUsage;
	DWORD WorkingSetSize;
	DWORD PeakWorkingSetSize;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	WTSProcessInfo()
	{}
	WTSProcessInfo(const WTS_PROCESS_INFO_EX& other) :pProcessName(other.pProcessName)
	{
		SessionId = other.SessionId;
		ProcessId = other.ProcessId;
		NumberOfThreads = other.NumberOfThreads;
		HandleCount = other.HandleCount;
		PagefileUsage = other.PagefileUsage;
		PeakPagefileUsage = other.PeakPagefileUsage;
		WorkingSetSize = other.WorkingSetSize;
		PeakWorkingSetSize = other.PeakWorkingSetSize;
		UserTime = other.UserTime;
		KernelTime = other.KernelTime;
	}
	WTSProcessInfo& operator=(const WTS_PROCESS_INFO_EX& other)
	{
		SessionId = other.SessionId;
		ProcessId = other.ProcessId;
		pProcessName = other.pProcessName;
		NumberOfThreads = other.NumberOfThreads;
		HandleCount = other.HandleCount;
		PagefileUsage = other.PagefileUsage;
		PeakPagefileUsage = other.PeakPagefileUsage;
		WorkingSetSize = other.WorkingSetSize;
		PeakWorkingSetSize = other.PeakWorkingSetSize;
		UserTime = other.UserTime;
		KernelTime = other.KernelTime;
		return *this;
	}
};