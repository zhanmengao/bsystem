#pragma once
#include<Windows.h>
#include<stdio.h>
#include<string>
#include"../../../C++space/Convert/Convert/Convert.h"
#include<strsafe.h>
#include"../../../C++space/StringTools/StringTools/StringHead.h"
#include<comdef.h>
static bool ErrorMessage(DWORD errCode, TCHAR* outBuffer, size_t& size)
{
	HLOCAL lpvMessageBuffer;
	DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	DWORD lvSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errCode, systemLocale, (TCHAR*)&lpvMessageBuffer, 0, NULL);
	if (!lvSize)
	{
		HMODULE hDLL = LoadLibraryEx(TEXT("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (hDLL)
		{
			lvSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				hDLL, errCode, systemLocale, (TCHAR*)&lpvMessageBuffer, 0, NULL);
		}
	}
	bool lvRet = memcpy_s(outBuffer, size, lpvMessageBuffer, lvSize * sizeof(TCHAR)) == 0;
	size = lvSize;
	if (lpvMessageBuffer != NULL)
	{
		LocalFree(lpvMessageBuffer);
	}
	return true;
}
static unsigned GetErrInfo(ULONG info, char* buf, int& len)
{
	len = 0;
	TCHAR tBuf[1024] = { 0 };
	size_t nSize = 1024;
	if (!ErrorMessage(info, tBuf, nSize))
	{
		return 1;
	}
	else
	{
#ifdef _UNICODE
		std::string str = Convert::WStringToString(tBuf);
		strcpy(buf, str.c_str());
#else
		strcpy(buf, tBuf);
#endif
	}
	//TO UTF8
	std::string utf8Code = Convert::PChar2GolangBuf(buf);
	strcpy(buf, utf8Code.c_str());
	len = strlen(buf);
	return 0;
}


class ErrorUtils
{
public:
	static stdstr ErrorMessage(DWORD error)
	{
		TCHAR buffer[1024] = { 0 };
		size_t size = sizeof(buffer);
		::ErrorMessage(error, buffer, size);
		return buffer;
	}
	static stdstr ComErrorMessage(HRESULT hr)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		return errMsg;
	}
};