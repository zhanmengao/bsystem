/*
���ƣ�ϵͳ�������
���ԣ�C++
���ܣ���Windowsϵͳ�����״̬��ȡ��������ͣ��������ֹͣ��������
*/
#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <windows.h>
#include <Winsvc.h>
#include <cstdio>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")
void OnBnClickedButton1()
{
	// �򿪷���������
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		printf("open SCManager error");
		return;
	}
	// ��www����
	SC_HANDLE hSvc = ::OpenService(hSC, L"W3SVC",
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		printf("Open www erron��");
		::CloseServiceHandle(hSC);
		return;
	}
	// ��÷����״̬
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		printf("Get Service state error��");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	//�������ֹͣ״̬���������񣬷���ֹͣ����
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// ֹͣ����
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			printf("stop service error��");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// �ȴ�����ֹͣ
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				printf("stop success��");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	else if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// ��������
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			printf("start service error��");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// �ȴ���������
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				printf("start success��");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	printf("start error��");
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return;
}