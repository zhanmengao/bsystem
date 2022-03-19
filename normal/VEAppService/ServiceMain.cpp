// ServiceTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "stdio.h"
#include "tchar.h"
#include "GStringClass.h"
#include "SocketMsg.h"
#include "utils.h"

#include "disk_config.h"
#include "protocal_struct.h"
#include "user_managerment.h"

#include "SocketMsg.h"
#include "GCDirFile.h"

#include "gclog.h"
//#include "compression_utility.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//定义全局函数变量
void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
void RunMode();
void LogEvent(LPCTSTR pszFormat, ...);
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);

TCHAR szServiceName[] = _T("VEAppService");
BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
DWORD dwThreadID;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
*
*/
//#include "../easyloggingpp/easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#define  APPLICATION   "VEAppService"

#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")  
//#pragma comment( //linker, "/subsystem:windows /ENTRY:mainCRTStartup") 

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
#ifdef _DEBUG

//

#endif //_DEBUG


	//user_managerment::instance()->create_users(protocal_message{});
	/**
	UtilsTools utilsTools;
	char file_path[260];
	tm s_last_time = utilsTools.get_current_time();
	sprintf_s(file_path, "%s\\%sLog\\%04d-%02d-%02d.log", utilsTools.get_application_dir().c_str(), "VEAppService",s_last_time.tm_year + 1900, s_last_time.tm_mon + 1, s_last_time.tm_mday);

	el::Configurations conf;
	conf.setToDefault();
	conf.setGlobally(el::ConfigurationType::Filename, file_path);
	el::Loggers::reconfigureLogger("default", conf);
	**/

	WriteLog(  "%s /install to install the service\n", APPLICATION );
	WriteLog(  "%s /uninstall to remove the service\n", APPLICATION);


	Init();

	dwThreadID = ::GetCurrentThreadId();

    SERVICE_TABLE_ENTRY st[] =
    {
        { szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

	if (_stricmp(lpCmdLine, "/install") == 0)
	{
		Install();
	
	}
	else if (_stricmp(lpCmdLine, "/uninstall") == 0)
	{
		Uninstall();
	}
	else
	{
		if (!::StartServiceCtrlDispatcher(st))
		{
			LogEvent(_T("Register Service Main Function Error!"));
			////LOG(INFO)<< "Register Service Main Function Error!";

			/**
			* 服务未安装，未加任何启动参数时直接运行业务
			*/
			RunMode();
		}
		else
		{
			LogEvent(_T("Register Service Main Function OK!"));
		}
	}

	return 0;
}

//*********************************************************
//Functiopn:			Init
//Description:			初始化
//*********************************************************
void Init()
{
    hServiceStatus = NULL;
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status.dwCurrentState = SERVICE_STOPPED;
    status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    status.dwWin32ExitCode = 0;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
}

//*********************************************************
//Functiopn:			ServiceMain
//Description:			服务主函数
//*********************************************************
void WINAPI ServiceMain()
{
    // Register the control request handler
    status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	//注册服务控制
    hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
    if (hServiceStatus == NULL)
    {
        LogEvent(_T("Handler not installed"));
        return;
    }
    SetServiceStatus(hServiceStatus, &status);

    status.dwWin32ExitCode = S_OK;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	RunMode();

    status.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hServiceStatus, &status);
    LogEvent(_T("Service stopped"));
}

//*********************************************************
//Functiopn:			ServiceStrl
//Description:			服务控制主函数
//*********************************************************
void WINAPI ServiceStrl(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
		StopAccept = 1;
		status.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(hServiceStatus, &status);

		status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatus, &status);
        PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
        break;
    case SERVICE_CONTROL_PAUSE:
        break;
    case SERVICE_CONTROL_CONTINUE:
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        break;
    default:
        LogEvent(_T("Bad service request"));
    }
}
//*********************************************************
//Functiopn:			IsInstalled
//Description:			判断服务是否已经被安装
//*********************************************************
BOOL IsInstalled()
{
    BOOL bResult = FALSE;

	//打开服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
		//打开服务
        SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

//*********************************************************
//Functiopn:			Install
//Description:			安装服务
//*********************************************************
BOOL Install()
{
    if (IsInstalled())
        return TRUE;

	//打开服务控制管理器
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务
    SC_HANDLE hService = ::CreateService(
        hSCM, szServiceName, szServiceName,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, _T(""), NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), szServiceName, MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

//*********************************************************
//Functiopn:			Uninstall
//Description:			删除服务
//*********************************************************
BOOL Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务
    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if (bDelete)
        return TRUE;

	LogEvent(_T("Service could not be deleted"));
    return FALSE;
}

//*********************************************************
//Functiopn:			RunMode
//Description:			服务运行
//*********************************************************
void RunMode()
{	
	//LOG(INFO)<< "Start  "<< APPLICATION;
	WriteLog("Start %s\n" , APPLICATION );
	SocketInit();
}


//*********************************************************
//Functiopn:			LogEvent
//Description:			记录服务日志
//*********************************************************
void LogEvent(LPCTSTR pFormat, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
	
	hEventSource = RegisterEventSource(NULL, szServiceName);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}


