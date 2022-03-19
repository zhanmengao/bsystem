#pragma once
#ifndef CONVERT_HPP  
#define CONVERT_HPP 
#include<assert.h>
#include<stdio.h>
#include<time.h>

#ifdef _WIN32
#include<Windows.h>
#include<string>
#include<vector>
#include<sstream>
using namespace std;
class Convert
{
public:
	static std::string string_To_UTF8(const std::string & str);
	static std::string UTF8_To_string(const std::string & str);
	static string UnicodeToUtf8(const wstring& in_str);
	static string PChar2GolangBuf(const string& vcstr);
	static string GBKToUTF8(const char* strGBK);
	static LPWSTR Str2Unicode(char* pStrA);

	static string ASCIIToUtf8(string& strAsciiCode);
	static wstring ASCIIToWideByte(string& strascii);
	static string Utf8ToASCII(const string& strUtf8Code);
	static string UnicodeToASCII(const wstring& str);
	static string WideByteToASCII(const std::wstring& wstrcode);
	static wstring Utf8ToUnicode(const std::string& utf8string);

	static std::string WStringToString(const std::wstring &wstr);
	static std::wstring StringToWString(const std::string& str);

	static wstring MultiCharToWideChar(const string& in_str);
	static string  WideCharToMultiChar(const wstring& in_str);

	//返回堆内存，需要自己delete
	static char*  WideCharToMultiByte2(wchar_t* pWCStrKey);
	//返回堆内存，需要自己delete
	static wchar_t*  MultiByteToWideChar2(const string& pKey);				

	static UINT GetConsoleCP()
	{
		return GetConsoleOutputCP();
	}
	static BOOL SetConsoleCP(UINT code)
	{
		setlocale(LC_ALL, "chinese-simplified");
		return SetConsoleOutputCP(code);
	}
};

#endif

#endif
