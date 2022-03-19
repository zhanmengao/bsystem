// CreateProcessAsUser_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

#include <UserEnv.h>
#include <WtsApi32.h>
#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "WtsApi32.lib")


// ������ں����Լ�����ص�����
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv);
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode);
void DoTask();
// ��ʾ��Ϣ�Ի���
void ShowMessage(TCHAR *lpszMessage, TCHAR *lpszTitle);
// �����û�����
BOOL CreateUserProcess(char *lpszFileName);

// ȫ�ֱ���
char g_szServiceName[MAX_PATH] = "CreateProcessAsUser_Test.exe";    // �������� 
SERVICE_STATUS g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = { 0 };


int _tmain(int argc, _TCHAR* argv[])
{
	// ע�������ں���
	SERVICE_TABLE_ENTRY stDispatchTable[] = { { g_szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain }, { NULL, NULL } };
	::StartServiceCtrlDispatcher(stDispatchTable);
	system("pause");
	return 0;
}


void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv)
{
	g_ServiceStatus.dwServiceType = SERVICE_WIN32;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;

	g_ServiceStatusHandle = ::RegisterServiceCtrlHandler(g_szServiceName, ServiceCtrlHandle);

	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	::SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);

	// �Լ�����ʵ�ֲ��ִ����������
	DoTask();
}


void __stdcall ServiceCtrlHandle(DWORD dwOperateCode)
{
	switch (dwOperateCode)
	{
	case SERVICE_CONTROL_PAUSE:
	{
		// ��ͣ
		g_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	}
	case SERVICE_CONTROL_CONTINUE:
	{
		// ����
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	}
	case SERVICE_CONTROL_STOP:
	{
		// ֹͣ
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwCheckPoint = 0;
		g_ServiceStatus.dwWaitHint = 0;
		::SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
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


void DoTask()
{
	// �Լ�����ʵ�ֲ��ִ����������
	// ��ʾ�Ի���
	ShowMessage("Hi Demon��Gan\nThis Is From Session 0 Service!\n", "HELLO");
	// �����û��������
	CreateUserProcess("D:\\Space\\Safe\\KeyWordConsole\\Debug\\KeyWordConsole.exe");
}


void ShowMessage(TCHAR *lpszMessage, TCHAR *lpszTitle)
{
	// ��ȡ��ǰ��Session ID
	DWORD dwSessionId = ::WTSGetActiveConsoleSessionId();
	// ��ʾ��Ϣ�Ի���
	DWORD dwResponse = 0;
	::WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, dwSessionId,
		lpszTitle, (1 + ::lstrlen(lpszTitle)),
		lpszMessage, (1 + ::lstrlen(lpszMessage)),
		0, 0, &dwResponse, FALSE);
}


// ͻ��SESSION 0���봴���û�����
BOOL CreateUserProcess(char *lpszFileName)
{
	BOOL bRet = TRUE;
	DWORD dwSessionID = 0;
	HANDLE hToken = NULL;
	HANDLE hDuplicatedToken = NULL;
	LPVOID lpEnvironment = NULL;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);

	do
	{
		// ��õ�ǰSession ID
		dwSessionID = ::WTSGetActiveConsoleSessionId();

		// ��õ�ǰSession���û�����
		if (FALSE == ::WTSQueryUserToken(dwSessionID, &hToken))
		{
			ShowMessage("WTSQueryUserToken", "ERROR");
			bRet = FALSE;
			break;
		}

		// ��������
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			ShowMessage("DuplicateTokenEx", "ERROR");
			bRet = FALSE;
			break;
		}

		// �����û�Session����
		if (FALSE == ::CreateEnvironmentBlock(&lpEnvironment,
			hDuplicatedToken, FALSE))
		{
			ShowMessage("CreateEnvironmentBlock", "ERROR");
			bRet = FALSE;
			break;
		}

		// �ڸ��Ƶ��û�Session��ִ��Ӧ�ó��򣬴�������
		if (FALSE == ::CreateProcessAsUser(hDuplicatedToken,
			lpszFileName, NULL, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
			lpEnvironment, NULL, &si, &pi))
		{
			ShowMessage("CreateProcessAsUser", "ERROR");
			bRet = FALSE;
			break;
		}

	} while (FALSE);
	// �رվ��, �ͷ���Դ
	if (lpEnvironment)
	{
		::DestroyEnvironmentBlock(lpEnvironment);
	}
	if (hDuplicatedToken)
	{
		::CloseHandle(hDuplicatedToken);
	}
	if (hToken)
	{
		::CloseHandle(hToken);
	}
	return bRet;
}




