#include"Convert.h"
#include<windows.h>

std::string Convert::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

std::string Convert::UTF8_To_string(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
std::wstring Convert::StringToWString(const std::string& str)
{
	int num = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	return w_str;
}

std::string Convert::WStringToString(const std::wstring &wstr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	std::string str;
	str.resize(nLen, '\0');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	return str;
}

wstring Convert::MultiCharToWideChar(const string& in_str)
{
	wstring return_wstr;
	int len = MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), in_str.size(), NULL, 0);
	wchar_t *buffer = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, in_str.c_str(), in_str.size(), buffer, len);
	buffer[len] = '\0';
	return_wstr.append(buffer);
	delete[]buffer;
	buffer = NULL;
	return return_wstr;
}

wchar_t*  Convert::MultiByteToWideChar2(const string& pKey)
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

string Convert::UnicodeToUtf8(const wstring& in_str)
{
	string return_str;
	int len = WideCharToMultiByte(CP_UTF8, 0, in_str.c_str(), -1, NULL, 0, NULL, NULL);
	CHAR *buffer = new CHAR[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, in_str.c_str(), -1, buffer, len, NULL, NULL);
	buffer[len] = '\0';
	return_str.append(buffer);
	delete[]buffer;
	buffer = NULL;
	return return_str;
}

string Convert::PChar2GolangBuf(const string& vcstr)
{
	return UnicodeToUtf8(MultiCharToWideChar(vcstr));
}



char*  Convert::WideCharToMultiByte2(wchar_t* pWCStrKey)
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
string  Convert::WideCharToMultiChar(const wstring& in_str)
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

string Convert::GBKToUTF8(const char* strGBK)
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

LPWSTR Convert::Str2Unicode(char* pStrA)
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

//ASCII 转 Utf8
string Convert::ASCIIToUtf8(string& strAsciiCode)
{
	string strRet("");

	//先把 ascii 转为 unicode 
	wstring wstr = ASCIIToWideByte(strAsciiCode);

	//最后把 unicode 转为 utf8
	strRet = UnicodeToUtf8(wstr);

	return strRet;
}

//ASCII 转 Unicode
wstring Convert::ASCIIToWideByte(string& strascii)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		perror("Invalid UTF-8 sequence.");
		return L"";
	}
	if (widesize == 0)
	{
		perror("Error in conversion.");
		return L"";
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		perror("La falla!");
		return L"";
	}

	return std::wstring(&resultstring[0]);
}

string Convert::Utf8ToASCII(const string& strUtf8Code)
{
	string strRet;
	//先把 utf8 转为 unicode 
	wstring wstr = Utf8ToUnicode(strUtf8Code);
	//最后把 unicode 转为 ascii
	strRet = WideByteToASCII(wstr);

	return strRet;
}

//ASCII 转为 Unicode
string Convert::UnicodeToASCII(const wstring& str)
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

//Unicode 转 ASCII
string Convert::WideByteToASCII(const std::wstring& wstrcode)
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

std::wstring Convert::Utf8ToUnicode(const std::string& utf8string)
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

