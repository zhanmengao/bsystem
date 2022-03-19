#pragma once
#include<Windows.h>
#include<strsafe.h>
#include<tchar.h>
#include"StrUtils.h"

class WinStrUtils
{
public:
	//模式匹配
	static bool Find(const TCHAR* s, const TCHAR* t)
	{
		const TCHAR* f = _tcsstr(s, t);
		if (f == nullptr)
			return false;
		else
			return true;
	}
	//忽略大小写的模式匹配
	static bool Findi(const TCHAR* s, const TCHAR* t)
	{
		//全部转大写，然后比较
		stdstr sstr(s);
		stdstr tstr(t);
		for (auto it = sstr.begin(); it != sstr.end(); it++)
		{
			*it = towupper(*it);
		}
		for (auto it = tstr.begin(); it != tstr.end(); it++)
		{
			*it = towupper(*it);
		}
		return WinStrUtils::Find(s, t);
	}
	//字符串比较
	static bool Comparei(const TCHAR* s, const TCHAR* t)
	{
		return _tcsicmp(s, t) == 0;
	}
	//比较前n项
	static bool Compareni(const TCHAR* s, const TCHAR* t, size_t nSize)
	{
		return _tcsnicmp(s, t, nSize) == 0;
	}
	//拼接
	static bool SafeStrcat(TCHAR* pszDest, size_t cchDest, const TCHAR* pszSrc)
	{
		size_t remain;
		DWORD flag = 0;
		return StringCchCatEx(pszDest, cchDest, pszSrc,nullptr, &remain, flag);
	}
	//拷贝
	static bool SafeStrcpy(TCHAR* pszDest,size_t cchDest,const TCHAR* pszSrc)
	{
		size_t remain;
		DWORD flag = 0;
		return StringCchCopyEx(pszDest, cchDest, pszSrc, nullptr, &remain, flag);
	}
	//格式化
	static long SafeSprintf(TCHAR* pszDest, size_t cchDest,const TCHAR* format,...)
	{
		va_list argptr;
		va_start(argptr, format);
		size_t remain;
		DWORD flag = 0;
		HRESULT res = StringCchPrintfEx(pszDest, cchDest, nullptr, &remain, flag, format, argptr);
		va_end(argptr);
		return res;
	}
	static bool Compare(const TCHAR* s, unsigned slen, const TCHAR* t, unsigned tlen, DWORD dwFlag = NORM_IGNORECASE)
	{
		int ret = CompareString(GetThreadLocale(), dwFlag, s, slen, t, tlen);
		return CheckCompareStringRet(ret);
	}
	static bool CompareOrdinal(const wchar_t* s, unsigned slen, const wchar_t* t, unsigned tlen, bool bIgnoreCase = true)
	{
		int ret = CompareStringOrdinal(s, slen, t, tlen, bIgnoreCase);
		return CheckCompareStringRet(ret);
	}
	static std::wstring GUID2Str(GUID guid)
	{
		if (CoCreateGuid(&guid) == S_OK)
		{
			wchar_t test[48] = { 0 };
			StringFromGUID2(guid, test, sizeof(test));
			return test;
		}
		return L"";
	}
private:
	static bool CheckCompareStringRet(int ret)
	{
		if (ret == 0)
		{
			//error
			return 0;
		}
		else if (ret == CSTR_LESS_THAN)
		{
			return 0;
		}
		else if (ret == CSTR_EQUAL)
		{
			return 0;
		}
		else if (ret == CSTR_GREATER_THAN)
		{
			return 1;
		}
		return 0;
	}
};
/*
_tcsstr:字符串1在字符串2中首次出现的位置，未出现返回NULL值；
_tcsrchr：字符1 在字符串2中首次出现的位置，未出现返回NULL值
_tcslen(str) 获得字符串长度
_tcsrchr(str, L'\\') 反向搜索获得最后一个TCHAR的位置
_stprintf(TCHAR *buffer,const TCHAR *format [,argument] ... )获得一个格式化字符串
_tcsdup 给一个指针分配源字符串大小的内存并从源字符串copy值
_tcstok 按标记将字符串拆分
_tcscpy_s 拷贝字符串
*/