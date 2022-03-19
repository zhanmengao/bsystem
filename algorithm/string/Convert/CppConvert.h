#ifndef _CPPCONVERT_H
#define _CPPCONVERT_H
#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
static unsigned char* Buffer2Hex(const char *str,char* buffer)
{
	//×¢ÊÍ1
	int str_len = strlen(str);
	int i = 0;
	for (i = 0; i < str_len; i = i + 2)
	{
		sscanf(str + i, "%2hhx", &buffer[i / 2]);
	}
	return (unsigned char*)buffer;
}
static unsigned char* Str2Hex(const char *str)
{
	//×¢ÊÍ1
	unsigned char *ret = NULL;
	int str_len = strlen(str);
	int i = 0;
	ret = (unsigned char *)malloc(str_len / 2);
	for (i = 0; i < str_len; i = i + 2)
	{
		sscanf(str + i, "%2hhx", &ret[i / 2]);
	}
	return ret;
}
#ifdef _WIN32
static void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
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
#endif
static void Byte2Hex(unsigned char bData, unsigned char hex[])
{
	int high = bData / 16, low = bData % 16;
	hex[0] = (high <10) ? ('0' + high) : ('A' + high - 10);
	hex[1] = (low <10) ? ('0' + low) : ('A' + low - 10);
}
static unsigned char toHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

static unsigned char fromHex(unsigned char x)
{
	unsigned char y = 0;
	if (x >= 'A' && x <= 'Z')
		y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z')
		y = x - 'a' + 10;
	else if (x >= '0' && x <= '9')
		y = x - '0';
	else
		return y;
	return y;
}
#ifdef __cplusplus
#include<string>
#include<vector>
#include<sstream>
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include<stdexcept>
#include<iostream>
#include <memory>
class CppConvert
{
public:
	static std::string toHex(const char* hex, size_t nSize)
	{
		std::string str;
		char buf[8] = { 0 };
		for (size_t i = 0; i < nSize; i++)
		{
			sprintf(buf, "%02x", (unsigned char)hex[i]);
			str.append(buf, 2);
		}
		return str;
	}

	static std::string Hex2(const unsigned char* hex, size_t nSize)
	{
		std::string str;
		for (size_t i = 0; i < nSize; i++)
		{
			str[i] = fromHex(hex[i]);
		}
		return str;
	}

	static std::string UnicodeToUTF8(const std::wstring & wstr)
	{
		std::string ret;
		try
		{
			std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
			ret = wcv.to_bytes(wstr);
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << std::endl;
		}
		return ret;
	}
	static std::wstring UTF8ToUnicode(const std::string & str)
	{
		std::wstring ret;
		try
		{
			std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
			ret = wcv.from_bytes(str);
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << std::endl;
		}
		return ret;
	}
	static std::string UnicodeToANSI(const std::wstring & wstr)
	{
		std::string ret;
		std::mbstate_t state = {};
		const wchar_t *src = wstr.data();
		size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
		if (static_cast<size_t>(-1) != len)
		{
			std::unique_ptr< char[] > buff(new char[len + 1]);
			len = std::wcsrtombs(buff.get(), &src, len, &state);
			if (static_cast<size_t>(-1) != len)
			{
				ret.assign(buff.get(), len);
			}
		}
		return ret;
	}
	static std::wstring ANSIToUnicode(const std::string & str)
	{
		std::wstring ret;
		std::mbstate_t state = {};
		const char *src = str.data();
		size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
		if (static_cast<size_t>(-1) != len)
		{
			std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
			len = std::mbsrtowcs(buff.get(), &src, len, &state);
			if (static_cast<size_t>(-1) != len)
			{
				ret.assign(buff.get(), len);
			}
		}
		return ret;
	}
	static std::string UTF8ToANSI(const std::string & str)
	{
		return UnicodeToANSI(UTF8ToUnicode(str));
	}
	static std::string ANSIToUTF8(const std::string & str)
	{
		return UnicodeToUTF8(ANSIToUnicode(str));
	}
};
#endif
#endif
