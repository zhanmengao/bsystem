#pragma once

#ifdef _WIN64

#include <json_x64\json.h>

#ifdef _DEBUG
#pragma comment(lib, "./curl-7.59.0_x64/lib/libcurl-d_imp.lib")
#else
#pragma comment(lib, "./curl-7.59.0_x64/lib/libcurl_imp.lib")
#endif
#else
#include <json\json.h>

#endif

#include <string>
#include <Windows.h>


using namespace std;

class WindowApp
{
public:
	WindowApp();
	~WindowApp();

	int GetInstallAppList(Json::Value&);
	int GetVEAppList(Json::Value&);

	int AppendVEApp(Json::Value);
	int DeleteVEApp(Json::Value);


private:

	bool EnumShellLink( wchar_t*  strSrc, Json::Value& items, int flag = 1);	
	bool GetShortCutFile( wchar_t* ShortcutFile, wchar_t * filePath, int sizefile, wchar_t * argv, int sizeargv);	
	bool is_exe_file(wchar_t * filename);

	bool ReadTSAppAllowList(Json::Value& items);

	int DelTSAppRegKey(Json::Value DelItems);

	int PutTSAppRegKey(Json::Value AddItems);
};

