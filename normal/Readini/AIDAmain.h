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
		//���ƥ��
		const DATA& row = it->second;
		//ģʽƥ��
		if (row["matchStyle"] == _T("1"))
		{
			if (val.find(row["Str"]) != string::npos)
			{
				return true;
			}
		}
		//ȫƥ��
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
#define PATH1 "D:\\Space\\System\\SystemInfo\\VEIDP\\����-С�׵��ӻ�.ini"
#define PATH2 "D:\\Space\\System\\SystemInfo\\VEIDP\\����2.ini"
	iniManager mINIHW(PATH2);
	TCHAR keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		IDPMonit info;
		sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|��ʾ������", (i + 1));
		info.brand = mINIHW.ReadString("��ʾ��", keyName);
		if (!IsInvalidAIDAValue(info.brand))
		{
			sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|���ֱ���", (i + 1));
			info.imageSize = mINIHW.ReadString("��ʾ��", keyName);

			sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|��������Ļ", (i + 1));
			info.size = mINIHW.ReadString("��ʾ��", keyName);

			sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|��ʾ�� ID", (i + 1));
			info.ID = mINIHW.ReadString("��ʾ��", keyName);

			sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|�ͺ�", (i + 1));
			info.model = mINIHW.ReadString("��ʾ��", keyName);

			sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|������", (i + 1));
			info.maker = mINIHW.ReadString("��ʾ��", keyName);

			sprintf_s(keyName, _countof(keyName), "��ʾ��%d|��ʾ������|���к�", (i + 1));
			info.SerialNumber = mINIHW.ReadString("��ʾ��", keyName);

			std::cout << info.brand << info.imageSize << info.size << info.ID << info.model << info.maker
				<< info.SerialNumber << endl;
		}
	}
	return 0;
}