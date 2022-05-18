#pragma once
#include <iostream>
#include <windows.h>
#include<tchar.h>
#include<wchar.h>
#include<string>
#include"../../../C++space/StringTools/StringTools/StrUtils.h"

using namespace std;

struct INI_KV
{
	stdstr Key;
	stdstr Val;
};
class iniManager
{
public:
	iniManager()
	{

	}
	iniManager(const stdstr& iniPath):iniPath(iniPath)
	{
	}
	void SetPath(const stdstr& str)
	{
		iniPath = str;
	}
	stdstr ReadString(const TCHAR* appName,const TCHAR* keyName,const TCHAR* defaultVal = _T("*"),
		DWORD *pdwRet = nullptr)
	{
		TCHAR buf[MAX_PATH] = { 0 };
		DWORD dwRet = GetPrivateProfileString(appName, keyName, defaultVal, buf, sizeof(buf), iniPath.c_str());
		if (pdwRet)
		{
			*pdwRet = dwRet;
		}
		return buf;
	}
	void ReadSection(const TCHAR* appName,std::vector<INI_KV> &val)
	{
		TCHAR temp[10240] = { 0 };
		DWORD dwRet = GetPrivateProfileSection(appName, temp, sizeof(temp), iniPath.c_str());
		DWORD offset = 0;
		while (offset < dwRet)
		{
			INI_KV kv;
			stdstr str = temp + offset;
			vector<stdstr> sv;
			StrUtils::Split(str, sv, _T('='));
			//Æ«ÒÆ
			offset += str.size() + 1;
			if (sv.size() >= 2)
			{
				kv.Key = sv[0];
				kv.Val = sv[1];
			}
			else
			{
				continue;
			}
			val.push_back(kv);
		}
	}
	void ReadSectionNames(std::vector<stdstr> &val)
	{
		TCHAR temp[40960] = { 0 };
		DWORD dwRet = GetPrivateProfileSectionNames(temp, sizeof(temp), iniPath.c_str());
		DWORD offset = 0;
		while (offset < dwRet)
		{
			stdstr str = temp + offset;
			//Æ«ÒÆ
			offset += str.size() + 1;

			val.push_back(str);
		}
	}
	UINT ReadNum(const stdstr& appName, const stdstr& keyName,UINT defaultVal = 0)
	{
		return GetPrivateProfileInt(appName.c_str(), keyName.c_str(), defaultVal, iniPath.c_str());
	}
	BOOL WriteString(const stdstr& appName, const stdstr& keyName, const stdstr& Val)
	{
		return WritePrivateProfileString(appName.c_str(), keyName.c_str(), Val.c_str(), iniPath.c_str());	
	}
	BOOL WriteNum(const stdstr& appName, const stdstr& keyName, UINT Val)
	{
		return WritePrivateProfileString(appName.c_str(), keyName.c_str(), TOSTR(Val).c_str(), iniPath.c_str());
	}
private:
	stdstr iniPath;
};
