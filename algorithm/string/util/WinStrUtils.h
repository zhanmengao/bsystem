#pragma once
#include<Windows.h>
#include<strsafe.h>
#include<tchar.h>
#include"StrUtils.h"

class WinStrUtils
{
public:
	//ģʽƥ��
	static bool Find(const TCHAR* s, const TCHAR* t)
	{
		const TCHAR* f = _tcsstr(s, t);
		if (f == nullptr)
			return false;
		else
			return true;
	}
	//���Դ�Сд��ģʽƥ��
	static bool Findi(const TCHAR* s, const TCHAR* t)
	{
		//ȫ��ת��д��Ȼ��Ƚ�
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
	//�ַ����Ƚ�
	static bool Comparei(const TCHAR* s, const TCHAR* t)
	{
		return _tcsicmp(s, t) == 0;
	}
	//�Ƚ�ǰn��
	static bool Compareni(const TCHAR* s, const TCHAR* t, size_t nSize)
	{
		return _tcsnicmp(s, t, nSize) == 0;
	}
	//ƴ��
	static bool SafeStrcat(TCHAR* pszDest, size_t cchDest, const TCHAR* pszSrc)
	{
		size_t remain;
		DWORD flag = 0;
		return StringCchCatEx(pszDest, cchDest, pszSrc,nullptr, &remain, flag);
	}
	//����
	static bool SafeStrcpy(TCHAR* pszDest,size_t cchDest,const TCHAR* pszSrc)
	{
		size_t remain;
		DWORD flag = 0;
		return StringCchCopyEx(pszDest, cchDest, pszSrc, nullptr, &remain, flag);
	}
	//��ʽ��
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
_tcsstr:�ַ���1���ַ���2���״γ��ֵ�λ�ã�δ���ַ���NULLֵ��
_tcsrchr���ַ�1 ���ַ���2���״γ��ֵ�λ�ã�δ���ַ���NULLֵ
_tcslen(str) ����ַ�������
_tcsrchr(str, L'\\') ��������������һ��TCHAR��λ��
_stprintf(TCHAR *buffer,const TCHAR *format [,argument] ... )���һ����ʽ���ַ���
_tcsdup ��һ��ָ�����Դ�ַ�����С���ڴ沢��Դ�ַ���copyֵ
_tcstok ����ǽ��ַ������
_tcscpy_s �����ַ���
*/