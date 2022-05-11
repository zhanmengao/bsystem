//NET_API_STATUS NET_API_FUNCTION NetLocalGroupAdd(
//	LPCWSTR servername,
//	DWORD   level,
//	LPBYTE  buf,
//	LPDWORD parm_err
//);
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <windows.h> 
#include <lm.h>
#define GROUP L"AAA"

void main5()
{
	_LOCALGROUP_INFO_0 info;
	info.lgrpi0_name = GROUP;
	NetLocalGroupAdd(NULL, 0, (LPBYTE)&info,NULL);

	NetLocalGroupSetInfo(NULL, GROUP, 0, (LPBYTE)&info, NULL);
	NetLocalGroupDel(NULL, GROUP);
}