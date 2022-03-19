#pragma once
/*
������ֻ��ʾ�ػ���ע�����������ֹ��ܣ���ҪĿ����������Ȩ�޵Ŀ��ƣ��������ܿ����MSDN�ĵ�˵��
*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <conio.h>
#include <ctype.h>
#include <iostream>
#include <windows.h>

using namespace std;

/*
��������Ȩ��
*/
static bool improvePv()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) return false;
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) return false;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, NULL, NULL, NULL)) return false;
	return true;
}

/*
�ػ�
*/
static bool powerOffProc()
{
	if (!improvePv() || !ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) return false;
	return true;
}

/*
ע��
*/
static bool logOffProc()
{
	if (!improvePv() || !ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) return false;
	return true;
}

/*
����
*/
static bool reBootProc()
{
	if (!improvePv() || !ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) return false;
	return true;
}


class PowerOffUtils
{
public:
	static bool PowerOff()
	{
		return ::powerOffProc();
	}
	static bool Reboot()
	{
		return ::reBootProc();
	}
	static bool LogOff()
	{
		return ::logOffProc();
	}
};