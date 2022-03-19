#pragma once

#include<vector>
#include<string>
#include<Windows.h>
#include<set>
#include"../../../../C++space/StringTools/StringTools/StrUtils.h"
using namespace std;

typedef struct Application
{
	stdstr mName;
	stdstr mRegName;
	stdstr mPath;
	stdstr mRequiredCommandLine;
	bool operator<(const Application &other) const
	{
		return mName < other.mName;
	}
};
typedef struct _RegApplication
{
	stdstr DisplayName;
	stdstr DisplayVersion;
	stdstr Publisher;
	stdstr InstallDate;
}RegApplication, PRegApplication;
class AppManager
{
public:
	static int GetSoftListInfo(vector<RegApplication>& vec);
	static DWORD GetInstallAppList(set<Application>& s);
private:
	static bool EnumShellFolder(TCHAR*  strSrc, set<Application>& s, int flag);
	//获取快捷方式指向的exe
	static BOOL GetShortCutFile(TCHAR* ShortcutFile, TCHAR* filePath, int sizefile, TCHAR* argv, int sizeargv);
	//msi方式注册的快捷方式 找到其exe
	static BOOL GetMsiShortCutTarget(const TCHAR* strShortcutFile, TCHAR* strTargetFile, DWORD& nSize);
	//是否exe
	static bool is_exe_file(TCHAR* filename);
};

