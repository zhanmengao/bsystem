#pragma once
#include<Windows.h>
#include "tlhelp32.h"
#include<stdio.h>
#include"../../../C++space/StringTools/StringTools/StringHead.h"

#define LOAD "LoadLibraryA"
#define FREE "FreeLibrary"
#include<tchar.h>
#include"ExpDefine.h"



class DLLUtils
{
public:
	static void InjectDLLThread(DWORD pid, const char* dllPath);								//将DLL注入指定进程
	static void InjectDLLThread(DWORD pid, RemoteThreadProc proc);						//无dll 直接注入代码段
	static void InjectDLLAPC(DWORD pid, const char* dllPath);								//将DLL注入指定进程
	static void FreeAllDLL(const TCHAR* inDllPath);
	static DWORD FreeDLL(DWORD pid, const TCHAR* inDllPath);							//将DLL从指定进程析构
	// 使用 ZwCreateThreadEx 实现远线程注入
	static BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName);

	//创建DLL寄宿程序
	static BOOL CreateDLL(const HANDLE& hToken, const stdstr& dllPath,STARTUPINFO& si, PROCESS_INFORMATION& pi);
	//DLL
	static LPVOID Malloc(HANDLE hProcess, SIZE_T nSize);
	static BOOL WriteMemory(HANDLE hProcess, LPVOID to, LPVOID from, SIZE_T nSize);
	static HMODULE LoadDLL(const TCHAR* dPath);
	static FARPROC GetProc(HMODULE hDLL, const char* procName);
	static HMODULE GetModule(const TCHAR* mName);						//获取模块句柄（通过模块名）
	static HMODULE GetModuleEx(PCTSTR func);														//获取当前代码模块句柄
	static stdstr _GetModuleFileName(HMODULE hModule);					//获取模块的真正路径
public:
	//修改PEB中的路径和命令行信息，实现进程伪装
	static BOOL DisguiseProcess(DWORD dwProcessID, TCHAR* lpwszPath, TCHAR* lpwszCmd);
	//创建傀儡进程
	static BOOL ReplaceProcess(const TCHAR *pszFilePath, PVOID pReplaceData, 
		DWORD dwReplaceDataSize, DWORD dwRunOffset);
};