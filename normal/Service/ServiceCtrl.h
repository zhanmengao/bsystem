#pragma once
#include <iostream>
#include <windows.h>
#include <Winsvc.h>
#include <cstdio>
#include <cstring>
#include<vector>
#include<functional>
#include"../../../C++space/StringTools/StringTools/StringHead.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;
typedef struct _ServicesInfo
{
	stdstr serviceName;
	stdstr displayName;
	SERVICE_STATUS status;
	_ServicesInfo(const ENUM_SERVICE_STATUSW& other):serviceName(other.lpServiceName),displayName(other.lpDisplayName),status(other.ServiceStatus)
	{
	}
	_ServicesInfo& operator=(const ENUM_SERVICE_STATUSW& other) 
	{
		serviceName = other.lpServiceName;
		displayName = other.lpDisplayName;
		status = (other.ServiceStatus);
	}
};
class ServiceCtrl
{
#define SERVICE_OP_ERROR -1
#define SERVICE_ALREADY_RUN -2
#define MAX_COUNT 255
#define SERVICE_NAME _T("smve")
public:
	static int APIENTRY StartUpService(const stdstr& servicePath);
	static int StopService();
	static int RemoveService();
	static SERVICE_STATUS GetServiceState();
	static BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress);
	static BOOL GetServiceList(vector<_ServicesInfo>& vec);
public:
	static SERVICE_STATUS_HANDLE g_hServiceStatus;
};
static void WINAPI ServiceControl(DWORD dwCode);
static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpArgv);
void DoTask();