#pragma once
#define ULONG unsigned long
#define EXPORT extern "C" __declspec(dllexport)

#define SESSION_LIST "SessionList"
#define PROCESS_LIST "ProcessList"

EXPORT ULONG GetSessionList(char* ret, int& len);

EXPORT ULONG GetProcessList(ULONG sid, char* ret, int& len);

EXPORT ULONG GetRemoteAPPList(ULONG sid, char* ret, int& len);

EXPORT ULONG KillProcess(ULONG pid);

EXPORT ULONG CreateEXE(ULONG sid,const char* exePath);

EXPORT ULONG SendMsg(ULONG sid, const char* content,const char* title,ULONG& response);

EXPORT ULONG Logoff(ULONG sid);

EXPORT unsigned ReturnErrInfo(ULONG info, char* buf, int& len);