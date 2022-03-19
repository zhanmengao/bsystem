#pragma once

#include <iostream>
#include <string> 
#include <Windows.h>

using namespace std;

class GStringClass
{

public: 
	
	static wchar_t * MultiByteToWideChar2(string pKey);
	static char * WideCharToMultiByte2(wchar_t * pWCStrKey);
	static string WideCharToMultiChar( wstring in_str );
	static wstring MultiCharToWideChar( string in_str );
	static string WideByteToASCII(const std::wstring & wstrcode);
	static string UnicodeToASCII(const wstring & str);
	static string Utf8ToASCII(string & strUtf8Code);
	static wstring ASCIIToWideByte(string & strascii);
	static char* UnicodeToUtf8(const wchar_t * unicode);
	static std::wstring Utf8ToUnicode(const std::string & utf8string);
	static std::string UnicodeToUtf8(const std::wstring & widestring);
	static string ASCIIToUtf8(string & strAsciiCode);	
	static string& replace_all(string& str, const string&  old_value, const string&  new_value);
	
	static void Hex2Str(const char *sSrc, char *sDest, int nSrcLen);	
	static void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);	
	static void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);


	BOOL GStringClass::StringToWString(const std::string &str, std::wstring &wstr);

	LPWSTR GStringClass::Str2Unicode(char* pStrA);
	char* GStringClass::CSting2PChar(CString Str);
	string GStringClass::FormatTime(__time64_t time64);
	string GStringClass::GBKToUTF8(const char* strGBK);

private:
	
	
	

	/*unsigned char GStringClass::ToHex(unsigned char x);
	unsigned char GStringClass::FromHex(unsigned char x);
	string GStringClass::UrlEncode(const string& str);
	string GStringClass::UrlDecode(const string& str);*/
};

