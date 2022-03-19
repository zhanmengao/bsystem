#pragma once
#include"../../WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../WindowsProcessHelper/CreateProcess/SessionDefine.h"
#include<vector>
#include<string>
#include<map>
#include "tlhelp32.h"
#include"../../../../C++space/LogManager/LogManager/LogManager.hpp"
#include"../../../Windows/DLLProject/DLL×¢Èë/DLLUtils.h"
#include"../../WindowsProcessHelper/CreateProcess/SessionManager.h"
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "UserEnv.lib")


class SessionForkUtils:public SessionManager
{
public:
	static std::vector<BOOL> CreateEXE_ALL_ME(const stdstr& exePath);
	static std::vector<BOOL> CreateEXE_ALL_Session(const stdstr& exePath);
	static DWORD CreateEXE_Session(const stdstr& exePath, DWORD session, bool bGuard = false, LogManager* log = nullptr);
	static DWORD CreateEXE_Me(const stdstr& exePath, DWORD session, bool bGuard = false, LogManager* log = nullptr);

	static std::vector<BOOL> CreateDLL_ALL_ME(const stdstr& dllPath);
	static std::vector<BOOL> CreateDLL_ALL_Session(const stdstr& dllPath);
	static BOOL CreateDLL_Session(const stdstr& exePath, DWORD session);
	static BOOL CreateDLL_Me(const stdstr& exePath, DWORD session);

	static BOOL WTSKillProcess(DWORD pid);
};