#pragma once
#include "Define.h"
#include"Tools.h"
#include"../../../Setting/Readini/INIManager.hpp"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../../../C++space/Convert/Convert/Convert.h"

class ConfigManager
{
public:
	static bool LoadConfig();
	static const string const GetLogPath()
	{
		return WorkDir() +"log.txt";
	}
	inline static std::string const WorkDir()
	{
		return mWorkDir;
	}
	inline static std::string const PicRoot()
	{
		return WorkDir() + "Pic/";
	}
	inline static unsigned short picWidth()
	{
		return mPicWidth;
	}
	inline static unsigned short picHeight()
	{
		return mPicHeight;
	}
	inline static unsigned short PicQual()
	{
		return mPicQual;
	}
	inline static string RootDir()
	{
		return mRootDir;
	}
	inline static wstring LockFile()
	{
		return L"C:\\ProgramData\\keylog\\" +
			ProcessHelper::GetProcessUser() + L"key.txt";
	}
protected:
	static const string const ConfigPath()
	{
		return RootDir() + "Config.ini";
	}
private:
	static iniManager miniManager;
	static unsigned short mPicWidth;
	static unsigned short mPicHeight;
	static unsigned short mPicQual;
	static string mRootDir;
	static string mWorkDir;
};