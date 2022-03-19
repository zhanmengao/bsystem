#pragma once
#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <windows.h>
#include <Winsvc.h>
#include <cstdio>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

class CEnablePriv
{
public:            //公有(对外开放的接口)

				   //
				   //设置当前进程优先级为最高(实时)
				   //
	static bool SetRealTimePriority();

	//
	//提升当前进程权限函数(“SeShutdownPrivilege”关机权限)
	//
	static bool EnableShutdownPriv();

	//
	//提升当前进程权限函数(“SeDebugPrivilege”读、写控制权限)
	//
	static bool EnableDebugPriv();

	//
	//提升当前进程权限函数(“SeBackupPrivilege”注册表备份权限)
	//
	static bool EnableBackupPriv();

	//
	//提升当前进程权限函数(“SeRestorePrivilege”恢复数据权限)
	//
	static bool EnableRestorePriv();

private:           //私有(内部使用的接口)

};
