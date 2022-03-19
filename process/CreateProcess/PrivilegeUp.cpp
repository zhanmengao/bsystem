#include"PrivilegeUp.h"
//宏的功能介绍:
//————————————————————-
//#define            SE_BACKUP_NAME               TEXT(“SeBackupPrivilege”)     //备份数据权限
//#define            SE_RESTORE_NAME              TEXT(“SeRestorePrivilege”)    //恢复数据权限
//#define            SE_SHUTDOWN_NAME             TEXT(“SeShutdownPrivilege”)   //关机权限
//#define            SE_DEBUG_NAME                TEXT(“SeDebugPrivilege”)      //读、写控制权限
//————————————————————-

//宏的功能介绍:
//————————————————————-
//ABOVE_NORMAL_PRIORITY_CLASS(0x00008000)   高于标准
//BELOW_NORMAL_PRIORITY_CLASS(0x00004000)   低于标准
//HIGH_PRIORITY_CLASS(0x00000080)           高
//IDLE_PRIORITY_CLASS(0x00000040)           低
//NORMAL_PRIORITY_CLASS(0x00000020)         标准
//REALTIME_PRIORITY_CLASS(0x00000100)       实时
//————————————————————-

//
//设置当前进程优先级为最高(实时)
//
//返回值：“false”是失败，“true”是成功。
bool CEnablePriv::SetRealTimePriority()
{
	if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
	{
		return false;
	}

	return true;
}

//
//提升当前进程权限函数(“SeShutdownPrivilege”关机权限)
//
//返回值：“false”是失败，“true”是成功。
bool CEnablePriv::EnableShutdownPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}

//
//提升当前进程权限函数(“SeDebugPrivilege”读、写控制权限)
//
//返回值：“false”是失败，“true”是成功。
bool CEnablePriv::EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);
	printf("Privailege : %i", GetLastError());
	return true;
}

//
//提升当前进程权限函数(“SeBackupPrivilege”备份数据权限)
//
//返回值：“false”是失败，“true”是成功。
bool CEnablePriv::EnableBackupPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}

//
//提升当前进程权限函数(“SeRestorePrivilege”恢复数据权限)
//
//返回值：“false”是失败，“true”是成功。
bool CEnablePriv::EnableRestorePriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}