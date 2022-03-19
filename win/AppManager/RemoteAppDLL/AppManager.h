#pragma once
#define ULONG unsigned long
#define EXPORT extern "C" __declspec(dllexport)
#define APP_LIST "AppList"
#define REMOTEAPP_LIST "RemoteAppList"

EXPORT ULONG GetAppList(char* buf, int& len);

EXPORT ULONG GetRemoteAppList(char* buf, int& len);

EXPORT ULONG InsertRemoteApp(const char* regName, const char* showName, const char* path, ULONG iconIndex,
	const char* iconPath, ULONG commandLineSetting, const char* requiredCommandLine, ULONG showInTSWA,
	const char* vPath, const char* securityDescriptor);

EXPORT ULONG DeleteRemoteApp(const char* regName);

EXPORT ULONG SetShowName(const char* regName, const char* val);

EXPORT ULONG SetPath(const char* regName, const char* val);

EXPORT ULONG SetIconIndex(const char* regName, ULONG val);

EXPORT ULONG SetIconPath(const char* regName, const char* val);

EXPORT ULONG SetCommandLineSetting(const char* regName, ULONG val);

EXPORT ULONG SetRequiredCommandLine(const char* regName, const char* val);

EXPORT ULONG SetShowInTSWA(const char* regName, ULONG val);

EXPORT ULONG SetVPath(const char* regName, const char* val);

EXPORT ULONG SetSecurityDescriptor(const char* regName, const char* val);

EXPORT unsigned ReturnErrInfo(ULONG info, char* buf, int& len);
//非导出函数
ULONG SetVal(const char* regName, const char* keyName, const char* val);
ULONG SetVal(const char* regName, const char* keyName, ULONG val);