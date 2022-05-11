#pragma once
#include<WinSock2.h>
#include<string>
#include<Windows.h>
#include"Convert.h"
#include"StringHead.h"
#include<lmwksta.h>
#include <lm.h>
#include<iostream>
#include <iphlpapi.h>
#include<tchar.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib,"netapi32.lib")

struct WindowsInfo
{
	DWORD   platform_id;
	wstring   computername;
	wstring    langroup;
	DWORD   dwMajVer;
	DWORD   dwMinVer;
	friend OUTSTREAM& operator<<(OUTSTREAM& os, const WindowsInfo& info)
	{
		std::wcout << L"platform_id:" << info.platform_id << L"	computername" << info.computername
			<< L"	langroup" << info.langroup
			<< L"	GetVersion:" << info.GetVersion() << endl;
		return os;
	}
	DWORD GetVersion() const
	{
		return (DWORD)MAKELONG(dwMinVer, dwMajVer);
	}
};
struct WindowsVersion
{
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	TCHAR  szCSDVersion[128] = { 0 };     // Maintenance string for PSS usage
	WindowsVersion(const OSVERSIONINFO& other) :dwOSVersionInfoSize(other.dwOSVersionInfoSize), dwMajorVersion(other.dwMajorVersion),
		dwMinorVersion(other.dwMinorVersion), dwBuildNumber(other.dwBuildNumber), dwPlatformId(other.dwPlatformId)
	{
		_tcscpy_s(szCSDVersion, _countof(szCSDVersion), other.szCSDVersion);
	}
	WindowsVersion& operator=(const OSVERSIONINFO& other)
	{
		dwOSVersionInfoSize = (other.dwOSVersionInfoSize);
		dwMajorVersion = (other.dwMajorVersion);
		dwMinorVersion = (other.dwMinorVersion);
		dwBuildNumber = (other.dwBuildNumber);
		dwPlatformId = (other.dwPlatformId);
		_tcscpy_s(szCSDVersion, _countof(szCSDVersion), other.szCSDVersion);
	}
	friend OUTSTREAM& operator<<(OUTSTREAM& os, const WindowsVersion& info)
	{
		STDOUT << _T("platform_id:") << info.dwPlatformId << _T("	dwBuildNumber") << info.dwBuildNumber
			<< L"	szCSDVersion" << info.szCSDVersion
			<< info.GetPlatformName()
			<< L"	dwMajorVersion:" << info.dwMajorVersion
			<< L"	dwMinorVersion:" << info.dwMinorVersion << endl;
		return os;
	}
	stdstr GetPlatformName() const
	{
		switch (dwPlatformId)
		{
		case VER_PLATFORM_WIN32s:
			return _T("WIN32s");
		case VER_PLATFORM_WIN32_WINDOWS:
			return _T("Windows 95 ,Windows 98");
		case VER_PLATFORM_WIN32_NT:
			return _T("Windows NT,Windows 2000,Windows XP,Windows Server2003,Windows Vista");
		}
		return _T("Unknow Windows Platform");
	}
};
struct net_info
{
	std::string strIp;
	std::string strMac;
};
typedef struct AdapterInfo
{
	string strName; // ÊÊÅäÆ÷Ãû³Æ
	string strDriverDesc; // ÊÊÅäÆ÷ÃèÊö
	string strIP; // IPµØÖ·
	string strNetMask; // ×ÓÍøÑÚÂë
	string strNetGate; // Íø¹Ø
	string strBroadcastIp; // ¹ã²¥µØÖ·
	string strS;
};
struct TCPPort
{
	DWORD dwState;
	CHAR	szLocalAddr[32] = { 0 };
	DWORD   dwLocalPort;
	CHAR	szRemoteAddr[32] = { 0 };
	DWORD       dwRemotePort;
	DWORD       dwOwningPid;
	std::string GetIpPortStr()
	{
		char usLocalPortSendBuf[64] = { 0 };
		sprintf(usLocalPortSendBuf, "%s:%d", szLocalAddr, dwLocalPort);
		return usLocalPortSendBuf;
	}
	std::string GetStateStr()
	{
		CHAR	szState[32] = { 0 };
		switch (dwState)
		{
		case MIB_TCP_STATE_CLOSED:
			sprintf_s(szState, _countof(szState), "%s", "CLOSED");
			break;
		case MIB_TCP_STATE_LISTEN:
			sprintf_s(szState, _countof(szState), "%s", "LISTENING");
			break;
		case MIB_TCP_STATE_SYN_SENT:
			sprintf_s(szState, _countof(szState), "%s", "SYN_SENT");
			break;
		case MIB_TCP_STATE_SYN_RCVD:
			sprintf_s(szState, _countof(szState), "%s", "SYN_RCVD");
			break;
		case MIB_TCP_STATE_ESTAB:
			sprintf_s(szState, _countof(szState), "%s", "ESTABLISHED");
			break;
		case MIB_TCP_STATE_FIN_WAIT1:
			sprintf_s(szState, _countof(szState), "%s", "FIN_WAIT_1");
			break;
		case MIB_TCP_STATE_FIN_WAIT2:
			sprintf_s(szState, _countof(szState), "%s", "FIN_WAIT_2");
			break;
		case MIB_TCP_STATE_CLOSE_WAIT:
			sprintf_s(szState, _countof(szState), "%s", "CLOSE_WAIT");
			break;
		case MIB_TCP_STATE_CLOSING:
			sprintf_s(szState, _countof(szState), "%s", "CLOSING");
			break;
		case MIB_TCP_STATE_LAST_ACK:
			sprintf_s(szState, _countof(szState), "%s", "LAST_ACK");
			break;
		case MIB_TCP_STATE_TIME_WAIT:
			sprintf_s(szState, _countof(szState), "%s", "TIME_WAIT");
			break;
		case MIB_TCP_STATE_DELETE_TCB:
			sprintf_s(szState, _countof(szState), "%s", "DELETE_TCB");
			break;
		}
		return szState;
	}
};
struct UDPPort
{
	CHAR	szLocalAddr[32] = { 0 };
	DWORD       dwLocalPort;
	DWORD       dwOwningPid;
	std::string GetIpPortStr()
	{
		char usLocalPortSendBuf[64] = { 0 };
		sprintf(usLocalPortSendBuf, "%s:%d", szLocalAddr, dwLocalPort);
		return usLocalPortSendBuf;
	}
};
struct EventLog
{
	DWORD  EventID;
	DWORD EventType;
	DWORD TimeGenerated;
	DWORD  TimeWritten;
	EventLog(const EVENTLOGRECORD& obj) :TimeGenerated(obj.TimeGenerated), TimeWritten(obj.TimeWritten)
	{
		EventID = (obj.EventID); //& 0xFFFF;
		EventType = obj.EventType;
	}
	EventLog& operator=(const EVENTLOGRECORD& obj)
	{
		EventType = obj.EventType;
		EventID = obj.EventID; //& 0xFFFF;
		TimeGenerated = obj.TimeGenerated;
		TimeWritten = obj.TimeWritten;
		return *this;
	}
	stdstr GetEventTypeStr()
	{
		switch (EventType)
		{
		case 1:
			return _T("Error");
		case 2:
			return _T("Warning");
			break;
		case 3:
			return _T("Informational");
			break;
		case 4:
			return _T("Audit Success");
		case 5:
			return _T("Audit Failure");
			break;
		}
		return _T("");
	}
};