#pragma once
#include"SessionDefine.h"
#include<vector>
#include<string>
#include<map>
#include "tlhelp32.h"
#include"ProcessHelper.h"
#include"../../../../C++space/LogManager/LogManager/LogManager.hpp"
#include"../../../Windows/DLLProject/DLL×¢Èë/DLLUtils.h"
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "UserEnv.lib")
using namespace std;

class SessionManager
{
public:
	static HANDLE OpenServer(TCHAR* sName);
	static void CloseServer(HANDLE hServer);
	static stdstr GetInitProgram(TCHAR* sName, TCHAR* uName);
	static BOOL SetInitProgram(TCHAR* sName, TCHAR* uName, TCHAR* initProgram);

	static DWORD GetSessionID(const TCHAR* user);
	static DWORD GetSessionID(DWORD pid);
	static DWORD GetSessionID();
	static DWORD GetActiveSessionID();
	static vector<DWORD>  GetActiveSessionList();
	static BOOL GetWTSSessions(vector<RDPLoginSessionInfo>& vec);

	static RDPLoginUserInfo GetLoginUserInfo(DWORD SessionId);
	static BOOL GetWTSProcesses(DWORD SessionId, map<DWORD, WTSProcessInfo> &promap);

	static BOOL GetRemoteAPPBySID(DWORD SessionId, map<DWORD, ProcessInfo> &promap);
	static stdstr GetRemoteAPP(DWORD SessionId);
	static BOOL GetRemoteAPPByPID(DWORD ProcessID, map<DWORD, ProcessInfo> &promap);
	static BOOL GetRemoteAPPByPID(const std::map<DWORD, ProcessInfo>& _mapProcess,
		DWORD ProcessID, map<DWORD, ProcessInfo> &promap);

	static BOOL GetRDPinit(DWORD SessionId, WTSProcessInfo &rdpProcess,const TCHAR* rdpinit = _T("rdpi"));


	static HANDLE GetToken(DWORD session);

	static BOOL WTSSend(DWORD session, TCHAR* content, TCHAR* title,DWORD& response);
	static BOOL WTSDisconnect(DWORD session);
	static BOOL WTSLogoff(DWORD session);
	static bool IsInRDP(DWORD session = WTS_CURRENT_SESSION);
};