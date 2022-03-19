#include"ServiceCtrl.h"
#include <UserEnv.h>
#include <WtsApi32.h>
#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "WtsApi32.lib")
#include<tchar.h>

void WINAPI ServiceControl(DWORD dwCode)
{
	switch (dwCode)
	{
		//服务暂停
	case SERVICE_CONTROL_PAUSE:
	{
		// 暂停
		ServiceCtrl::TellSCM(SERVICE_PAUSE_PENDING, 0, 1);
		ServiceCtrl::TellSCM(SERVICE_PAUSED, 0, 0);
		break;
	}
		//服务继续
	case SERVICE_CONTROL_CONTINUE:
	{
		// 继续
		ServiceCtrl::TellSCM(SERVICE_CONTINUE_PENDING, 0, 1);
		ServiceCtrl::TellSCM(SERVICE_RUNNING, 0, 0);
		break;
	}
		//服务停止
	case SERVICE_CONTROL_STOP:
	{
		// 停止
		ServiceCtrl::TellSCM(SERVICE_STOP_PENDING, 0, 1);
		ServiceCtrl::TellSCM(SERVICE_STOPPED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
}
int APIENTRY ServiceCtrl::StartUpService(const stdstr& servicePath)
{
	SERVICE_TABLE_ENTRY svTable[] = 
	{
		{ SERVICE_NAME,ServiceMain },
		{ NULL,NULL }
	};
	StartServiceCtrlDispatcher(svTable);
	DWORD dwErrorCode;
	SERVICE_STATUS ssServiceStatus;
	SC_HANDLE hscManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hscManager)
	{
		printf("Can not Open the Service Manager\n");
		return SERVICE_OP_ERROR;
	}
	printf("Service Manager Opened Success\n");
	SC_HANDLE hServiceHandle = CreateService(hscManager, SERVICE_NAME, SERVICE_NAME,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, 
		SERVICE_ERROR_IGNORE, servicePath.c_str(), NULL, NULL, NULL, NULL, NULL);
	if (!hServiceHandle)
	{
		dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_SERVICE_EXISTS)
		{
			hServiceHandle = OpenService(hscManager, SERVICE_NAME, SERVICE_ALL_ACCESS);
			if (!hServiceHandle)
			{
				printf("Can not Create/Open Service\n");
				CloseServiceHandle(hServiceHandle);
				return SERVICE_OP_ERROR;
			}
			else
			{
				printf("Service Opened Success\n");
			}
		}
	}
	else 
	{
		printf("Service Create Success\n");
	}
	if (!StartService(hServiceHandle, 0, NULL))
	{
		dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_SERVICE_ALREADY_RUNNING)
		{
			printf("SERVEICE IS ALREADY RUNNING\n");
			CloseServiceHandle(hServiceHandle);
			CloseServiceHandle(hscManager);
			return SERVICE_ALREADY_RUN;
		}
		else
		{
			printf("SERVEICE START ERROR %u\n", dwErrorCode);
			CloseServiceHandle(hServiceHandle);
			CloseServiceHandle(hscManager);
			return SERVICE_OP_ERROR;
		}
	}
	while (QueryServiceStatus(hServiceHandle, &ssServiceStatus))
	{
		if (ssServiceStatus.dwCurrentState == SERVICE_START_PENDING)
		{
			Sleep(100);
			continue;
		}
		if (ssServiceStatus.dwCurrentState != SERVICE_RUNNING)
		{
			printf("Service Start Process ERROR\n");
			CloseServiceHandle(hServiceHandle);
			CloseServiceHandle(hscManager);
			return SERVICE_OP_ERROR;
		}
		else
		{
			break;
		}
	}
	if (!QueryServiceStatus(hServiceHandle, &ssServiceStatus))
	{
		printf("Service Status Get Error\n");
		CloseServiceHandle(hServiceHandle);
		CloseServiceHandle(hscManager);
		return SERVICE_OP_ERROR;
	}
	printf("Service Start Success\n");
	CloseServiceHandle(hServiceHandle);
	CloseServiceHandle(hscManager);
	return 0;
}
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpArgv)
{
	OutputDebugStringA("ServiceMain Call");
	ServiceCtrl::g_hServiceStatus = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceControl);
	ServiceCtrl::TellSCM(SERVICE_START_PENDING, 0, 1);
	ServiceCtrl::TellSCM(SERVICE_RUNNING, 0, 0);
	while (true)
	{
		Sleep(5000);
		DoTask();
	}
}
int ServiceCtrl::StopService()
{
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		printf("open SCManager error");
		return -1;
	}
	// 打开服务
	SC_HANDLE hSvc = ::OpenService(hSC, SERVICE_NAME,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		printf("Open service erron。");
		::CloseServiceHandle(hSC);
		return -1;
	}
	// 获得服务的状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		printf("Get Service state error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return -1;
	}
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			printf("stop service error。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return -1;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				printf("stop success。");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return 0;
			}
		}
	}
	return 0;
}
int ServiceCtrl::RemoveService()
{
	SC_HANDLE hscManager;
	SC_HANDLE hServiceHandle;
	SERVICE_STATUS ssServiceStatus;
	hscManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hscManager)
	{
		printf("Open Service Manager Error\n");
		return SERVICE_OP_ERROR;
	}
	printf("Open Service Manager Success\n");
	hServiceHandle = OpenService(hscManager, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (!hServiceHandle)
	{
		printf("Open Service Error\n");
		return SERVICE_OP_ERROR;
	}
	printf("Open Service Success\n");
	if (QueryServiceStatus(hServiceHandle, &ssServiceStatus))
	{
		if (ssServiceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			ControlService(hServiceHandle, SERVICE_CONTROL_STOP, &ssServiceStatus);
		}
	}
	else
	{
		printf("Service Status Get Error\n");
		CloseServiceHandle(hServiceHandle);
		CloseServiceHandle(hscManager);
		return SERVICE_OP_ERROR;
	}
	if (!DeleteService(hServiceHandle))
	{
		printf("Delete Service Error\n");
		CloseServiceHandle(hServiceHandle);
		CloseServiceHandle(hscManager);
		return SERVICE_OP_ERROR;
	}
	printf("Remove Service Success\n");
	CloseServiceHandle(hServiceHandle);
	CloseServiceHandle(hscManager);
	return 0;
}
SERVICE_STATUS_HANDLE ServiceCtrl::g_hServiceStatus;


SERVICE_STATUS ServiceCtrl::GetServiceState()
{
	SC_HANDLE hSC = nullptr;
	SC_HANDLE hSvc = nullptr;
	// 获得服务的状态
	SERVICE_STATUS status;
	do
	{
		// 打开服务管理对象
		SC_HANDLE hSC = ::OpenSCManager(NULL,
			NULL, GENERIC_EXECUTE);
		if (hSC == NULL)
		{
			printf("open SCManager error");
			return status;
		}
		// 打开服务
		SC_HANDLE hSvc = ::OpenService(hSC, SERVICE_NAME,
			SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if (hSvc == NULL)
		{
			printf("Open www erron。");
			::CloseServiceHandle(hSC);
			return status;
		}

		if (::QueryServiceStatus(hSvc, &status) == FALSE)
		{
			printf("Get Service state error。");
		}
	} while (0);
	if (hSvc)
	{
		::CloseServiceHandle(hSvc);
	}
	if (hSC)
	{
		::CloseServiceHandle(hSC);
	}
	return status;
}

BOOL ServiceCtrl::TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress)
{
	SERVICE_STATUS serviceStatus = { 0 };
	BOOL bRet = FALSE;

	::RtlZeroMemory(&serviceStatus, sizeof(serviceStatus));
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwState;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = dwExitCode;
	serviceStatus.dwWaitHint = 3000;

	bRet = ::SetServiceStatus(g_hServiceStatus, &serviceStatus);
	return bRet;
}
BOOL ServiceCtrl::GetServiceList(vector<_ServicesInfo>& vec)
{
	SC_HANDLE hscManager = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (!hscManager)
	{
		printf("Can not Open the Service Manager\n");
		return SERVICE_OP_ERROR;
	}
	DWORD dwBuffSize = 0;
	DWORD dwSuccessCount = 0;
	DWORD dwSec = 0;
	EnumServicesStatus(hscManager, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, dwBuffSize, &dwBuffSize, &dwSuccessCount, &dwSec);
	//获取缓冲区大小
	LPENUM_SERVICE_STATUSW buffer = (LPENUM_SERVICE_STATUSW)malloc(dwBuffSize);
	EnumServicesStatus(hscManager, SERVICE_WIN32, SERVICE_STATE_ALL, buffer, dwBuffSize, &dwBuffSize, &dwSuccessCount, &dwSec);
	for (unsigned i = 0; i < dwSuccessCount; i++)
	{
		vec.push_back(buffer[i]);
	}
	if (buffer)
	{
		free(buffer);
		buffer = nullptr;
	}
	return TRUE;
}