#pragma once

#include<json\json.h>
#include <string>
#include <Windows.h>
#pragma comment(lib,"jsoncpp_static.lib")

using namespace std;

class WindowApp
{
public:

	static int GetInstallAppList(Json::Value&);
	static int GetVEAppList(Json::Value&);

	static int AppendVEApp(Json::Value);
	static int DeleteVEApp(Json::Value);


private:

	static bool EnumShellLink( wchar_t*  strSrc, Json::Value& items, int flag = 1);					//枚举目录下所有快捷方式
	static bool GetShortCutFile( wchar_t* ShortcutFile, wchar_t * filePath, int sizefile, wchar_t * argv, int sizeargv);
	static bool is_exe_file(wchar_t * filename);

	static bool ReadTSAppAllowList(Json::Value& items);
	static int DelTSAppRegKey(Json::Value DelItems);
	static int PutTSAppRegKey(Json::Value AddItems);
};

