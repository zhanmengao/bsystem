#pragma once
#include<Windows.h>
#include<vector>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
class RegeditUtils
{
public:
	//×ÔÆô¶¯
	static LSTATUS RegAutoRun_CurrentUser(const TCHAR* lpName, const TCHAR* lpFilePath);
	static LSTATUS ShowRunList(std::vector<stdstr>& vec);
	static LSTATUS DelAutoRun_CurrentUser(const TCHAR* lpKeyValue);
public:
	static LSTATUS GetKeyVal(const HKEY& hMain, const TCHAR* key,std::vector<stdstr> &vec);
public:
	static LSTATUS SetKeyVal(const HKEY& hMain, const TCHAR* key,const TCHAR* newKey, const TCHAR* valName, const TCHAR* val);
	static LSTATUS SetVal(const HKEY& hKey, const TCHAR* key, DWORD val);
	static LSTATUS SetVal(const HKEY& hKey, const TCHAR* key, const TCHAR* val);
	static LSTATUS GetVal(const HKEY& hKey, const TCHAR* key, DWORD& val);
	static LSTATUS GetVal(const HKEY& hKey, const TCHAR* key, TCHAR* val, DWORD dataSize, DWORD& retSize);
};