#include "stdafx.h"
#include "GStringClass.h"
#include "string.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <assert.h>

BOOL GStringClass::StringToWString(const std::string &str, std::wstring &wstr)
{
	int nLen = (int)str.length();
	wstr.resize(nLen, L'\0');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}


wchar_t*  GStringClass::MultiByteToWideChar2(string pKey)
{
	//string 转 char*
	char* pCStrKey = (char*)pKey.c_str();

	//第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t *pWCStrKey = new wchar_t[pSize];

	//第二次调用将单字节字符串转换成双字节字符串
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);

	return pWCStrKey;
}


char*  GStringClass::WideCharToMultiByte2(wchar_t* pWCStrKey)
{
	//第一次调用确认转换后单字节字符串的长度，用于开辟空间
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];

	//第二次调用将双字节字符串转换成单字节字符串
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;

	//如果想要转换成string，直接赋值即可
	//string pKey = pCStrKey;
}


//宽字符 转 多字节
string  GStringClass::WideCharToMultiChar(wstring in_str)
{
	string return_str;
	int len = WideCharToMultiByte(CP_ACP, 0, in_str.c_str(), in_str.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, in_str.c_str(), in_str.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';

	return_str.append(buffer);
	delete[]buffer;
	return return_str;
}

//多字节 转 宽字符
wstring  GStringClass::MultiCharToWideChar(string in_str)
{
	wstring return_wstr;
	int len = MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), in_str.size(), NULL, 0);
	TCHAR *buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), in_str.size(), buffer, len);
	buffer[len] = '\0';
	return_wstr.append(buffer);
	delete[]buffer;
	return return_wstr;
}


//UTF-8 转 Unicode
std::wstring GStringClass::Utf8ToUnicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(widesize);

	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}

//Unicode 转 ASCII
string GStringClass::WideByteToASCII( const std::wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);

	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}

	return std::string(&resultstring[0]);
}

//ASCII 转为 Unicode
string GStringClass::UnicodeToASCII(const wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}


//UTF-8 转 ASCII
string GStringClass::Utf8ToASCII(string& strUtf8Code)
{
	string strRet;
	//先把 utf8 转为 unicode 
	wstring wstr = Utf8ToUnicode(strUtf8Code);
	//最后把 unicode 转为 ascii
	strRet = WideByteToASCII(wstr);

	return strRet;
}

///////////////////////////////////////////////////////////////////////

//ASCII 转 Unicode
wstring GStringClass::ASCIIToWideByte( string& strascii )
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}

//Unicode 转 Utf8
char* GStringClass::UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

//Unicode 转 Utf8
std::string GStringClass::UnicodeToUtf8(const std::wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<char> resultstring(utf8size);

	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if ( convresult != utf8size )
	{
		throw std::exception("La falla!");
	}

	return std::string(&resultstring[0]);
}

//ASCII 转 Utf8
string GStringClass::ASCIIToUtf8(string& strAsciiCode)
{
	string strRet("");
	
	//先把 ascii 转为 unicode 
	wstring wstr = ASCIIToWideByte(strAsciiCode);

	//最后把 unicode 转为 utf8
	strRet = UnicodeToUtf8(wstr);

	return strRet;
}



void GStringClass::Hex2Str(const char *sSrc, char *sDest, int nSrcLen)
{
	int  i;
	char szTmp[3];

	for (i = 0; i < nSrcLen; i++)
	{
		sprintf(szTmp, "%02X", (unsigned char)sSrc[i]);
		memcpy(&sDest[i * 2], szTmp, 2);
	}
	return;
}

void GStringClass::ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
	short i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i++)
	{
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return;
}

void GStringClass::HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{
	short i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i += 2)
	{
		highByte = toupper(source[i]);
		lowByte = toupper(source[i + 1]);

		if (highByte > 0x39)
			highByte -= 0x37;
		else
			highByte -= 0x30;

		if (lowByte > 0x39)
			lowByte -= 0x37;
		else
			lowByte -= 0x30;

		dest[i / 2] = (highByte << 4) | lowByte;
	}
	return;
}

LPWSTR GStringClass::Str2Unicode(char* pStrA)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, pStrA, -1, NULL, 0);

	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	//std::vector<wchar_t> resultstring(widesize);
	WCHAR* resultstring = new WCHAR[widesize];

	int convresult = MultiByteToWideChar(CP_ACP, 0, pStrA, -1, resultstring, widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	//return std::wstring(&resultstring[0]);
	return resultstring;

}

char* GStringClass::CSting2PChar(CString Str)
{
	char* tmpChar = new char[Str.GetLength()];
	memset(tmpChar, 0, Str.GetLength());

	int i;
	for (i = 0; i<Str.GetLength(); i++)
	{
		tmpChar[i] = (char)Str.GetAt(i);
	}
	tmpChar[i] = '\0';

	return tmpChar;
}


string GStringClass::FormatTime(__time64_t time64) {
	
	struct tm *info;
	char buffer[20] = { 0 };

	
	info = localtime(&time64);
	string timeStr(asctime(info));

	//localtime_s(&info, &time64);
	//sprintf(buffer, "%d年%d月%d日 %d时%d分%d秒", info.tm_year + 1900, info.tm_mon + 1, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec);
	size_t  index = timeStr.find_last_of("\n");
	if (index != timeStr.npos) {
		timeStr = timeStr.substr(0, index);
	}
	return timeStr;
}

string GStringClass::GBKToUTF8(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;

	if (wstr) delete[] wstr;
	if (str) delete[] str;

	return strTemp;
}

string& GStringClass::replace_all(string& str, const string&  old_value, const string&  new_value)
{
	while (true) {
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}

//unsigned char GStringClass::ToHex(unsigned char x)
//{
//	return  x > 9 ? x + 55 : x + 48;
//}
//
//unsigned char GStringClass::FromHex(unsigned char x)
//{
//	unsigned char y;
//	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
//	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
//	else if (x >= '0' && x <= '9') y = x - '0';
//	else assert(0);
//	return y;
//}
//
//string GStringClass::UrlEncode(const string& str)
//{
//	string strTemp = "";
//	size_t length = str.length();
//	for (size_t i = 0; i < length; i++)
//	{
//		if (isalnum((unsigned char)str[i]) ||
//			(str[i] == '-') ||
//			(str[i] == '_') ||
//			(str[i] == '.') ||
//			(str[i] == '~'))
//			strTemp += str[i];
//		else if (str[i] == ' ')
//			strTemp += "+";
//		else
//		{
//			strTemp += '%';
//			strTemp += ToHex((unsigned char)str[i] >> 4);
//			strTemp += ToHex((unsigned char)str[i] % 16);
//		}
//	}
//	return strTemp;
//}
//
//string GStringClass::UrlDecode(const string& str)
//{
//	string strTemp = "";
//	size_t length = str.length();
//	for (size_t i = 0; i < length; i++)
//	{
//		if (str[i] == '+') strTemp += ' ';
//		else if (str[i] == '%')
//		{
//			assert(i + 2 < length);
//			unsigned char high = FromHex((unsigned char)str[++i]);
//			unsigned char low = FromHex((unsigned char)str[++i]);
//			strTemp += high * 16 + low;
//		}
//		else strTemp += str[i];
//	}
//	return strTemp;
//}
