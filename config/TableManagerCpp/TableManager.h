#pragma once
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include"../../../C++space/StringTools/StringTools/StrUtils.h"
#include"../../../C++space/Convert/Convert/CppConvert.h"
#ifdef _WIN32
#include"../../../C++space/Convert/Convert/Convert.h"
#else
typedef char TCHAR;
#define _T(a) a
#endif
class DATA
{
public:
	DATA(const std::map<stdstr, stdstr>& rData): mData(rData)
	{

	}
	DATA& operator=(const std::map<stdstr, stdstr>& rData)
	{
		mData = rData;
	}
	stdstr operator[](const char* key) const
	{
		auto it = mData.find(key);
		if (it == mData.end())
		{
			return "";
		}
		else
		{
			return it->second;
		}
	}
private:
	std::map<stdstr, stdstr> mData;
};

typedef std::map<int, DATA> DATA_MAP;

class TableManager
{
public:
	bool Load(const char* nPath)
	{
		//先读字段名
		std::ifstream fs(nPath);
		if (!fs.is_open())
		{
			return false;
		}
		char buffer[1024] = { 0 };
		std::vector<stdstr> names;
		while (fs.getline(buffer, sizeof(buffer)))
		{
			size_t size = strlen(buffer);
			//注释掉的行不读
			if (size == 0 || buffer[0] == '#')
			{
				continue;
			}
#ifdef _UNICODE
			stdstr str = Convert::StringToWString(buffer);
#else
			stdstr str = buffer;
#endif
			//字符串切割
			std::vector<stdstr> datasp;
			StrUtils::Split(str, datasp, _T('	'));
			//该行解析失败
			if (datasp.size() == 0)
			{
				continue;
			}
			//如果是字段名
			if (names.empty())
			{
				for (int i = 0; i < datasp.size(); i++)
				{
					names.push_back(datasp[i]);
				}
			}
			//如果是数据
			else
			{
				std::map<stdstr, stdstr> dataMap;
				for (int i = 0; i < names.size() && i<datasp.size(); i++)
				{
					dataMap.insert(std::make_pair(names[i], datasp[i]));
				}
				//插入完成，插入id
#ifndef _UNICODE
				int id = atoi(datasp[0].c_str());
#else
				int id = atoi(CppConvert::UnicodeToANSI(datasp[0]).c_str());
#endif
				if (id <= 0)
				{
					continue;
				}
				mDataMap.insert(std::make_pair(id, dataMap));
			}

		}
		fs.close();
		return true;
	}
public:
	const DATA_MAP& GetData() const
	{
		return mDataMap;
	}
	const DATA* GetDataByID(int id) const
	{
		auto it = mDataMap.find(id);
		if (it == mDataMap.end())
		{
			return nullptr;
		}
		return &(it->second);
	}
	const stdstr GetDataByID(int id,const TCHAR* name) const
	{
		const DATA *pData = GetDataByID(id);
		if (pData)
		{
			return (*pData)[name];
		}
		return _T("");
	}
protected:
	DATA_MAP mDataMap;			//map<项ID,map<字段名称，内容>>
};
