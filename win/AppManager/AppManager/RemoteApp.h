#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
using namespace std;
typedef struct _RemoteApp
{
	stdstr RegName;
	stdstr ShowName;
	stdstr Path;
	DWORD IconIndex;
	stdstr IconPath;
	DWORD CommandLineSetting;
	stdstr RequiredCommandLine;
	DWORD ShowInTSWA;
	stdstr VPath;
	stdstr SecurityDescriptor;
}RemoteApp, *PRemoteApp;

class RemoteAppUtils
{
public:
	static LSTATUS GetRemoteAppList(vector<RemoteApp>& appList);				//±éÀú

	static LSTATUS AppendRemoteApp(const RemoteApp& app);							//Ìí¼Ó

	static LSTATUS DeleteRemoteApp(const TCHAR* regName);							//É¾³ý

	static LSTATUS SetRemoteAppVal(const TCHAR* regName, const TCHAR* key, const TCHAR* val);
	static LSTATUS SetRemoteAppVal(const TCHAR* regName, const TCHAR* key, DWORD val);
};

#define REG_NAME_KEY "RegName"
#define NAME_KEY "Name"
#define PATH_KEY  "Path"
#define ICON_IDX_KEY "IconIndex"
#define ICON_PATH_KEY "IconPath"
#define COMM_LINE_SET_KEY "CommandLineSetting"
#define REQ_COMM_LINE_KEY "RequiredCommandLine"
#define SHOW_TSWA_KEY "ShowInTSWA"
#define VPATH_KEY "VPath"
#define SECU_DESC_KEY "SecurityDescriptor"