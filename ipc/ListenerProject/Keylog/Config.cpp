#include"Tools.h"
#include "Config.h"
#include<tchar.h>

#include"LogManager.h"


#define APP_NAME L"EVENT_LOG"
#include<direct.h>
string ConfigManager::mWorkDir;
string ConfigManager::mRootDir;
iniManager ConfigManager::miniManager;
unsigned short ConfigManager::mPicWidth;
unsigned short ConfigManager::mPicHeight;
unsigned short ConfigManager::mPicQual;
bool ConfigManager::LoadConfig()
{
	mRootDir = Tools::GetModuleFolder();
	mWorkDir = mRootDir + Convert::WStringToString(ProcessHelper::GetProcessUser()) + "\\";
	miniManager.SetPath(Convert::StringToWString(ConfigPath()));
	mPicWidth= miniManager.ReadNum(APP_NAME, L"PicWidth", 200);
	mPicHeight = miniManager.ReadNum(APP_NAME, L"PicHeight", 200);
	mPicQual = miniManager.ReadNum(APP_NAME, L"PicQual", 20);
	_mkdir(mWorkDir.c_str());
	_mkdir(PicRoot().c_str());
	return true;
}