#pragma once
#include<Windows.h>

class HandleUtils
{
public:
	//复制句柄到其他进程
	static HANDLE CopyHandle(HANDLE hSrcProcess, HANDLE hTargetProcess, HANDLE handle,
		DWORD access = 0, BOOL bInherit = FALSE, DWORD option = DUPLICATE_SAME_ACCESS);
	static DWORD GetHandleFlags(HANDLE handle);																		//获取句柄的当前标志
	static BOOL SetHandleInherit(HANDLE handle, bool bInherit);													//设置句柄是否可继承
	static bool GetHandleInherit(HANDLE handle);
	static BOOL SetHandleProtectFromClose(HANDLE handle, bool bInherit);								//设置句柄是否可关闭
	static SECURITY_ATTRIBUTES GetInheritSa();																				//获取安全结构_标志可继承
	static void MakeSa(SECURITY_ATTRIBUTES& sa,bool bInheritHandle);										//设置安全描述符

	static HANDLE dup2(HANDLE hSrcProcess, HANDLE hSrcHandle, HANDLE hTargetProcess);				//拷贝一个句柄
};