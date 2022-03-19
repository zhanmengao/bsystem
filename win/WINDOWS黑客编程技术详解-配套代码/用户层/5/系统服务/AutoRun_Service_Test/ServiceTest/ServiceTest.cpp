// ServiceTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>


// ������ں����Լ�����ص�����
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv);
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode);
BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress);
void DoTask();

// ȫ�ֱ���
char g_szServiceName[MAX_PATH] = "ServiceTest.exe";    // �������� 
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = { 0 };


int _tmain(int argc, _TCHAR* argv[])
{
	// ע�������ں���
	SERVICE_TABLE_ENTRY stDispatchTable[] = { { g_szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain }, { NULL, NULL } };
	::StartServiceCtrlDispatcher(stDispatchTable);

	return 0;
}


void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv)
{
	g_ServiceStatusHandle = ::RegisterServiceCtrlHandler(g_szServiceName, ServiceCtrlHandle);

	TellSCM(SERVICE_START_PENDING, 0, 1);
	TellSCM(SERVICE_RUNNING, 0, 0);

	// �Լ�����ʵ�ֲ��ִ����������
	// !!ע��!! �˴�һ��ҪΪ��ѭ��, �����ڹػ��ٿ��������(���ǵ������), ���ܴ����û�����
	while (TRUE)
	{
		Sleep(5000);
		DoTask();
	}
}


void __stdcall ServiceCtrlHandle(DWORD dwOperateCode)
{
	switch (dwOperateCode)
	{
	case SERVICE_CONTROL_PAUSE:
	{
		// ��ͣ
		TellSCM(SERVICE_PAUSE_PENDING, 0, 1);
		TellSCM(SERVICE_PAUSED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_CONTINUE:
	{
		// ����
		TellSCM(SERVICE_CONTINUE_PENDING, 0, 1);
		TellSCM(SERVICE_RUNNING, 0, 0);
		break;
	}
	case SERVICE_CONTROL_STOP:
	{
		// ֹͣ
		TellSCM(SERVICE_STOP_PENDING, 0, 1);
		TellSCM(SERVICE_STOPPED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_INTERROGATE:
	{
		// ѯ��
		break;
	}
	default:
		break;
	}
}

BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress)
{
	SERVICE_STATUS serviceStatus = { 0 };
	BOOL bRet = FALSE;

	::RtlZeroMemory(&serviceStatus, sizeof(serviceStatus));
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwState;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = dwExitCode;
	serviceStatus.dwWaitHint = 3000;

	bRet = ::SetServiceStatus(g_ServiceStatusHandle, &serviceStatus);
	return bRet;
}

void DoTask()
{
	// �Լ�����ʵ�ֲ��ִ����������
}