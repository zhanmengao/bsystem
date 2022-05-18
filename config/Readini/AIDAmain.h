#pragma once
#include"INIManager.hpp"
#include"../TableManager/TableManager.h"
static TableManager mInvalidVal;
bool IsInvalidAIDAValue(const string& val)
{
	//find
	if (val.empty())
	{
		return true;
	}
	const DATA_MAP& tData = mInvalidVal.GetData();
	for (auto it = tData.begin(); it != tData.end(); it++)
	{
		//逐个匹配
		const DATA& row = it->second;
		//模式匹配
		if (row["matchStyle"] == _T("1"))
		{
			if (val.find(row["Str"]) != string::npos)
			{
				return true;
			}
		}
		//全匹配
		else if (row["matchStyle"] == "2")
		{
			if (val == row["Str"])
			{
				return true;
			}
		}
	}
	return false;
}
int AidaMain()
{
	struct IDPMonit
	{
		stdstr brand;
		stdstr imageSize;
		stdstr size;
		stdstr model;
		stdstr SerialNumber;
		stdstr ID;
		stdstr maker;
	};
	mInvalidVal.Load(("D:\\Space\\System\\SystemInfo\\VEIDP\\InvalidVal.txt"));
#define PATH1 "D:\\Space\\System\\SystemInfo\\VEIDP\\报告-小米电视机.ini"
#define PATH2 "D:\\Space\\System\\SystemInfo\\VEIDP\\报告2.ini"
	iniManager mINIHW(PATH2);
	TCHAR keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		IDPMonit info;
		sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|显示器名称", (i + 1));
		info.brand = mINIHW.ReadString("显示器", keyName);
		if (!IsInvalidAIDAValue(info.brand))
		{
			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|最大分辨率", (i + 1));
			info.imageSize = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|最大可用屏幕", (i + 1));
			info.size = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|显示器 ID", (i + 1));
			info.ID = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|型号", (i + 1));
			info.model = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|制造商", (i + 1));
			info.maker = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|序列号", (i + 1));
			info.SerialNumber = mINIHW.ReadString("显示器", keyName);

			std::cout << info.brand << info.imageSize << info.size << info.ID << info.model << info.maker
				<< info.SerialNumber << endl;
		}
	}
	return 0;
}