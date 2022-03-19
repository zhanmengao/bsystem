/*
名称：系统服务管理
语言：C++
介绍：对Windows系统服务的状态获取，服务暂停，开启，停止操作代码
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
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		printf("open SCManager error");
		return;
	}
	// 打开www服务。
	SC_HANDLE hSvc = ::OpenService(hSC, L"W3SVC",
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		printf("Open www erron。");
		::CloseServiceHandle(hSC);
		return;
	}
	// 获得服务的状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		printf("Get Service state error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
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
			return;
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
				return;
			}
		}
	}
	else if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			printf("start service error。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		// 等待服务启动
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				printf("start success。");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				return;
			}
		}
	}
	printf("start error。");
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return;
}