#include"../AppManager/Apps.h"
#include"../AppManager/RemoteApp.h"
#include"AppManager.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../../Network/Serialization/Json/JsonTools.hpp"
#include<strsafe.h>
#include<tchar.h>
#pragma comment(lib,"jsoncpp_static.lib")
ULONG GetAppList(char* ret, int& len)
{
	set<Application> appList;
	auto retCode = AppManager::GetInstallAppList(appList);

	Json::Value root;
	Json::Value& data = root[APP_LIST];
	unsigned i = 0;
	for (auto it = appList.begin(); it != appList.end();it++,i++)
	{
		data[i][REG_NAME_KEY] = Convert::UnicodeToUtf8(it->mRegName);
		data[i][NAME_KEY] = Convert::UnicodeToUtf8(it->mName);
		data[i][PATH_KEY] = Convert::UnicodeToUtf8(it->mPath);
		data[i][REQ_COMM_LINE_KEY] = Convert::UnicodeToUtf8(it->mRequiredCommandLine);
	}

	auto strWrite = JsonTools::GetJsonData(root);
	StringCchCopyA(ret, strlen(strWrite.c_str()), strWrite.c_str());
	len = strlen(ret);
	return retCode;
}

ULONG GetRemoteAppList(char* ret, int& len)
{
	Json::Value root;
	Json::Value& data = root[REMOTEAPP_LIST];
	vector<RemoteApp> appList;
	auto retCode = RemoteAppUtils::GetRemoteAppList(appList);
	for (int i = 0; i < appList.size(); i++)
	{
		data[i][REG_NAME_KEY] = Convert::UnicodeToUtf8(appList[i].RegName);
		data[i][NAME_KEY] = Convert::UnicodeToUtf8(appList[i].ShowName);
		data[i][PATH_KEY] = Convert::UnicodeToUtf8(appList[i].Path);
		data[i][ICON_IDX_KEY] = (long)appList[i].IconIndex;
		data[i][ICON_PATH_KEY] = Convert::UnicodeToUtf8(appList[i].IconPath);
		data[i][COMM_LINE_SET_KEY] = (long)(appList[i].CommandLineSetting);

		data[i][REQ_COMM_LINE_KEY] = Convert::UnicodeToUtf8(appList[i].RequiredCommandLine);
		data[i][SHOW_TSWA_KEY] = (long)(appList[i].ShowInTSWA);

		data[i][VPATH_KEY] = Convert::UnicodeToUtf8(appList[i].VPath);
		data[i][SECU_DESC_KEY] = Convert::UnicodeToUtf8(appList[i].SecurityDescriptor);
	}

	auto strWrite = JsonTools::GetJsonData(root);
	StringCchCopyA(ret, strlen(strWrite.c_str()), strWrite.c_str());
	len = strlen(ret);
	return retCode;
}

ULONG InsertRemoteApp(const char* regName, const char* showName, const char* path, ULONG iconIndex,
	const char* iconPath, ULONG commandLineSetting, const char* requiredCommandLine, ULONG showInTSWA,
	const char* vPath, const char* securityDescriptor)
{
	RemoteApp app;
	if (regName == NULL || strlen(regName) == 0)
	{
		return 13;
	}
	app.RegName = Convert::StringToWString(regName);
	if (showName != NULL && strlen(showName) > 0)
	{
		app.ShowName = Convert::StringToWString(showName);
	}

	if (path != NULL && strlen(path) > 0)
	{
		app.Path = Convert::StringToWString(path);
	}
	app.IconIndex = iconIndex;

	if (iconPath != NULL && strlen(iconPath) > 0)
	{
		app.IconPath = Convert::StringToWString(iconPath);
	}

	app.CommandLineSetting = commandLineSetting;

	if (requiredCommandLine != NULL && strlen(requiredCommandLine) > 0)
	{
		app.RequiredCommandLine = Convert::StringToWString(requiredCommandLine);
	}
	app.ShowInTSWA = showInTSWA;
	if (vPath != NULL && strlen(vPath) > 0)
	{
		app.VPath = Convert::StringToWString(vPath);
	}
	if (securityDescriptor != NULL && strlen(securityDescriptor) > 0)
	{
		app.SecurityDescriptor = Convert::StringToWString(securityDescriptor);
	}
	return RemoteAppUtils::AppendRemoteApp(app);
}
ULONG DeleteRemoteApp(const char* regName)
{
	wstring wstr = Convert::StringToWString(regName);
	return RemoteAppUtils::DeleteRemoteApp(wstr.c_str());
}
ULONG SetShowName(const char* regName, const char* val)
{
	return SetVal(regName, NAME_KEY, val);
}

EXPORT ULONG SetPath(const char* regName, const char* val)
{
	return SetVal(regName, PATH_KEY, val);
}
ULONG SetIconIndex(const char* regName, ULONG val)
{
	return SetVal(regName, ICON_IDX_KEY, val);
}

ULONG SetIconPath(const char* regName, const char* val)
{
	return SetVal(regName, ICON_PATH_KEY, val);
}

ULONG SetCommandLineSetting(const char* regName, ULONG val)
{
	return SetVal(regName, COMM_LINE_SET_KEY, val);
}

ULONG SetRequiredCommandLine(const char* regName, const char* val)
{
	return SetVal(regName, REQ_COMM_LINE_KEY, val);
}

ULONG SetShowInTSWA(const char* regName, ULONG val)
{
	return SetVal(regName, SHOW_TSWA_KEY, val);
}

ULONG SetVPath(const char* regName, const char* val)
{
	return SetVal(regName, VPATH_KEY, val);
}

ULONG SetSecurityDescriptor(const char* regName, const char* val)
{
	return SetVal(regName, SECU_DESC_KEY, val);
}
ULONG SetVal(const char* regName, const char* keyName, const char* val)
{
	wstring wReg = Convert::StringToWString(regName);
	wstring wKey = Convert::StringToWString(keyName);
	wstring wVal = Convert::StringToWString(val);
	return RemoteAppUtils::SetRemoteAppVal(wReg.c_str(), wKey.c_str(), wVal.c_str());
}
ULONG SetVal(const char* regName, const char* keyName, ULONG val)
{
	wstring wReg = Convert::StringToWString(regName);
	wstring wKey = Convert::StringToWString(keyName);
	return RemoteAppUtils::SetRemoteAppVal(wReg.c_str(), wKey.c_str(), val);
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