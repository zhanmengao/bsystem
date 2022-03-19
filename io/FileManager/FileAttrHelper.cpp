#include <stdio.h>
#include <bitset>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <iostream>
#include"FileAttrHelper.h"
#include<io.h>
#include<set>

BOOL FileAttrHelper::SetFileHidden(const stdstr& filePath, bool bHidden)
{
	DWORD attr = GetFileAttributes(filePath.c_str());
	//想显示
	if (IsHidden(attr) && !bHidden)
	{
		//运算规则：0^0=0；   0^1=1；   1^0=1；   1^1=0
		return SetFileAttributes(filePath.c_str(), attr ^ FILE_ATTRIBUTE_HIDDEN);
	}
	//想隐藏
	if (!IsHidden(attr) && bHidden)
	{
		//运算规则：0 | 0=0；   0 | 1=1；   1 | 0=1；   1| 1=0
		return SetFileAttributes(filePath.c_str(), attr | FILE_ATTRIBUTE_HIDDEN);
	}
	return TRUE;
}
BOOL FileAttrHelper::SetFileOnlyRead(const stdstr& filePath, bool bOnlyRead)
{
	DWORD attr = GetFileAttributes(filePath.c_str());
	//想取消
	if (IsOnlyRead(attr) && !bOnlyRead)
	{
		return SetFileAttributes(filePath.c_str(), attr ^ FILE_ATTRIBUTE_READONLY);
	}
	//想设置
	if (!IsOnlyRead(attr) && bOnlyRead)
	{
		return SetFileAttributes(filePath.c_str(), attr | FILE_ATTRIBUTE_READONLY);
	}
	return TRUE;
}
BOOL FileAttrHelper::SetFileNormal(const stdstr& filePath)
{
	return SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_NORMAL);
}
bool FileAttrHelper::IsFolder(unsigned attr)
{
	return attr & _A_SUBDIR;
}
bool FileAttrHelper::IsHidden(unsigned attr)
{
	return attr & _A_HIDDEN;
}
bool FileAttrHelper::IsOnlyRead(unsigned attr)
{
	return attr & _A_RDONLY;
}
bool FileAttrHelper::IsBinary(const std::string& nPath)
{
	static std::set<std::string> s;
	static bool bInit = false;
	if (!bInit)
	{
		s.insert(".exe");
		s.insert(".manifest");
		s.insert(".dll");
		s.insert(".x64");
		s.insert(".vsb");
		s.insert(".dat");
		s.insert(".mem");
		s.insert(".ia64");
		s.insert(".v64");
		s.insert(".w9x");
		bInit = true;
	}
    for (auto it = s.begin(); it != s.end(); it++)
	{
		if (nPath.find(*it) != std::string::npos)
		{
			return true;
		}
	}
	return false;
}
