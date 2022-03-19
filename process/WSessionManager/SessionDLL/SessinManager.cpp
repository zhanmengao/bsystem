#include"../../WindowsProcessHelper/CreateProcess/SessionManager.h"
#include"../../WSessionManager/SessionManager/SessionFork.h"
#include"../../WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include".. /../../../../../Network/Serialization/Json/JsonTools.hpp"
#include"SessionManager.h"
#include<json\json.h>
#include<string>
#include<strsafe.h>
#pragma comment (lib,"jsoncpp_static.lib")
unsigned long GetSessionList(char* ret, int& len)
{
	vector<RDPLoginSessionInfo> vec;
	BOOL bRet = SessionManager::GetWTSSessions(vec);
	if (!bRet)
	{
		return GetLastError();
	}
	Json::Value root;
	Json::Value& sData = root[SESSION_LIST];
	int index = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i].User.WTSUserName.empty())
		{
			continue;
		}
		sData[index]["SID"] = std::to_string(vec[i].sid);
		sData[index]["SName"] = Convert::UnicodeToUtf8(vec[i].sName);
		sData[index]["State"] = Convert::UnicodeToUtf8(vec[i].state);
		sData[index]["DeviceId"] = Convert::UnicodeToUtf8(vec[i].User.DeviceId);
		sData[index]["InitialProgram"] = Convert::UnicodeToUtf8(vec[i].User.WTSInitialProgram);
		sData[index]["ApplicationName"] = Convert::UnicodeToUtf8(vec[i].User.WTSApplicationName);
		sData[index]["WorkingDirectory"] = Convert::UnicodeToUtf8(vec[i].User.WTSWorkingDirectory);
		sData[index]["UserName"] = Convert::UnicodeToUtf8(vec[i].User.WTSUserName);
		sData[index]["WinStationName"] = Convert::UnicodeToUtf8(vec[i].User.WTSWinStationName);
		sData[index]["DomainName"] = Convert::UnicodeToUtf8(vec[i].User.WTSDomainName);
		sData[index]["ClientName"] = Convert::UnicodeToUtf8(vec[i].User.WTSClientName);
		sData[index]["ClientAddress"] = Convert::UnicodeToUtf8(vec[i].User.WTSClientAddress);
		sData[index]["ClientDirectory"] = Convert::UnicodeToUtf8(vec[i].User.WTSClientDirectory);
		index++;
	}
	string strWrite = JsonTools::GetJsonData(root);
	StringCchCopyA(ret, strlen(strWrite.c_str()), strWrite.c_str());
	len = strlen(ret);
	return 0;
}

ULONG GetProcessList(ULONG sid, char* ret, int& len)
{
	map<DWORD, WTSProcessInfo> promap;
	BOOL bRet = SessionManager::GetWTSProcesses(sid, promap);
	if (!bRet)
	{
		return GetLastError();
	}
	map<DWORD, ProcessInfo> allpromap;
	bRet = ProcessHelper::GetProcessList(allpromap);
	if (!bRet)
	{
		return GetLastError();
	}
	Json::Value root;
	Json::Value& data = root[PROCESS_LIST];
	unsigned idx = 0;
	for (auto it = promap.begin(); it != promap.end(); it++, idx++)
	{
		data[idx]["PID"] = std::to_string(it->first);
		data[idx]["PName"] = Convert::UnicodeToUtf8(it->second.pProcessName);
		data[idx]["NumberOfThreads"] = std::to_string(it->second.NumberOfThreads);
		data[idx]["HandleCount"] = std::to_string(it->second.HandleCount);
		data[idx]["PagefileUsage"] = std::to_string(it->second.PagefileUsage);
		data[idx]["PeakPagefileUsage"] = std::to_string(it->second.PeakPagefileUsage);
		auto fit = allpromap.find(it->first);
		if (fit != allpromap.end())
		{
			data[idx]["ExeFile"] = Convert::UnicodeToUtf8(fit->second.szExeFile);
			data[idx]["ParentPID"] = std::to_string(fit->second.th32ParentProcessID);
		}
		else
		{
			data[idx]["ExeFile"] = "";
			data[idx]["ParentPID"] = "";
		}
	}
	string strWrite = JsonTools::GetJsonData(root);
	StringCchCopyA(ret, strlen(strWrite.c_str()), strWrite.c_str());
	len = strlen(ret);
	return 0;
}

EXPORT ULONG GetRemoteAPPList(ULONG sid, char* ret, int& len)
{
	map<DWORD, ProcessInfo> promap;
	BOOL bRet = SessionManager::GetRemoteAPP(sid, promap);
	if (!bRet)
	{
		return GetLastError();
	}
	map<DWORD, WTSProcessInfo> sPromap;
	bRet = SessionManager::GetWTSProcesses(sid, sPromap);
	if (!bRet)
	{
		return GetLastError();
	}
	Json::Value root;
	Json::Value& data = root[PROCESS_LIST];
	unsigned idx = 0;
	for (auto it = promap.begin(); it != promap.end(); it++, idx++)
	{
		data[idx]["PID"] = Convert::PChar2GolangBuf(std::to_string(it->first));
		data[idx]["ExeFile"] = Convert::UnicodeToUtf8(it->second.szExeFile);
		data[idx]["ParentPID"] = Convert::PChar2GolangBuf(std::to_string(it->second.th32ParentProcessID));
		auto fit = sPromap.find(it->first);
		if (fit != sPromap.end())
		{
			data[idx]["PName"] = Convert::UnicodeToUtf8(fit->second.pProcessName);
			data[idx]["NumberOfThreads"] = Convert::PChar2GolangBuf(std::to_string(fit->second.NumberOfThreads));
			data[idx]["HandleCount"] = Convert::PChar2GolangBuf(std::to_string(fit->second.HandleCount));
			data[idx]["PagefileUsage"] = Convert::PChar2GolangBuf(std::to_string(fit->second.PagefileUsage));
			data[idx]["PeakPagefileUsage"] = Convert::PChar2GolangBuf(std::to_string(fit->second.PeakPagefileUsage));
		}
		else
		{
			data[idx]["PName"] = "";
			data[idx]["NumberOfThreads"] = "";
			data[idx]["HandleCount"] = "";
			data[idx]["PagefileUsage"] = "";
			data[idx]["PeakPagefileUsage"] = "";
		}
	}
	string strWrite = JsonTools::GetJsonData(root);
	StringCchCopyA(ret, strlen(strWrite.c_str()), strWrite.c_str());
	len = strlen(ret);
	return 0;
}


ULONG KillProcess(ULONG pid)
{
	BOOL bRet = SessionForkUtils::WTSKillProcess(pid);
	if (!bRet)
	{
		return GetLastError();
	}
	return 0;
}

ULONG CreateEXE(ULONG sid, const char* exePath)
{
	wstring wstr =  Convert::StringToWString(exePath);
	BOOL bRet = SessionForkUtils::CreateEXE_Session(wstr.c_str(), sid);
	if (!bRet)
	{
		return GetLastError();
	}
	return 0;
}

ULONG SendMsg(ULONG sid, const char* content, const char* title,ULONG& response)
{
	wstring wstr = Convert::Utf8ToUnicode(content);
	TCHAR bContent[1024] = { 0 };
	_tcscpy_s(bContent,_countof(bContent), wstr.c_str());

	wstr = Convert::Utf8ToUnicode(title);
	TCHAR bTitle[1024] = { 0 };
	_tcscpy_s(bTitle, _countof(bTitle), wstr.c_str());

	BOOL bRet = SessionManager::WTSSend(sid, bContent, bTitle, response);
	if (!bRet)
	{
		return GetLastError();
	}
	return 0;
}
ULONG Logoff(ULONG sid)
{
	BOOL bRet = SessionManager::WTSLogoff(sid);
	if (!bRet)
	{
		return GetLastError();
	}
	return 0;
}

bool SysErrorMessage(DWORD errCode, LPWSTR outBuffer, size_t& size)
{
	LPVOID lpvMessageBuffer;
	DWORD lvSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpvMessageBuffer, 0, NULL);
	bool lvRet = memcpy_s(outBuffer, size, lpvMessageBuffer, lvSize * sizeof(WCHAR)) == 0;
	size = lvSize;
	LocalFree(lpvMessageBuffer);
	return true;
}
unsigned ReturnErrInfo(ULONG info, char* buf, int& len)
{
	len = 0;
	TCHAR tBuf[1024] = { 0 };
	size_t nSize = 1024;
	if (!SysErrorMessage(info, tBuf, nSize))
	{
		return 1;
	}
	else
	{
		std::string str = Convert::WStringToString(tBuf);
		strcpy(buf, str.c_str());
	}
	//?utf8
	std::string utf8Code = Convert::PChar2GolangBuf(buf);
	strcpy(buf, utf8Code.c_str());
	len = strlen(buf);
	return 0;
}