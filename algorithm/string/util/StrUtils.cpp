#include"StrUtils.h"

 int Find(const char* haystack, const char* needle)
{
	const char* substr = strstr(haystack, needle);
	if (substr == NULL)
	{
		return -1;
	}
	else
	{
		return substr - haystack;
	}
}

unsigned Count(const char* haystack, size_t nSize, const char* needle)
{
	size_t ss = strlen(needle);
	unsigned nCount = 0;
	const char *temp = haystack;
	const char *pFind = NULL;
	while ((pFind = strstr(temp, needle)) != NULL)
	{
		nCount++;
		temp = pFind + ss;
		if (temp - haystack >= nSize - 1)
		{
			break;
		}
	}
	return nCount;
}

int replaceAll(char* string, size_t bufferSize, const char* src, const char* target,
	size_t* outSize)
{
	size_t nSize = strlen(string);
	size_t ss = strlen(src);
	size_t ts = strlen(target);
	int need = (int)ts - (int)ss;
	unsigned nCount = Count(string, nSize, src);
	if (nCount <= 0)
	{
		return 1;
	}
	unsigned needMemory = nSize + need*nCount + 1;
	if (bufferSize < needMemory)
	{
		return 0;
	}

	char* newStr = (char*)malloc(needMemory);
	memset(newStr, '\0', needMemory);

	char* temp = string;
	int nPos = Find(temp, src);
	size_t newIndex = 0;

	for (size_t i = 0; i < nSize;)
	{
		if (i < nPos)
		{
			newStr[newIndex++] = string[i++];
		}

		else
		{
			for (size_t j = 0; j < ts; j++)
			{
				newStr[newIndex++] = target[j];
			}

			i += ss;
			temp = string + i;
			nPos = Find(temp, src);
			if (nPos == -1)
			{
				strcpy(newStr + newIndex, temp);
				break;
			}
			nPos += i;
		}
	}
	strcpy(string, newStr);
	if (outSize)
	{
		*outSize = needMemory;
	}
	free(newStr);
	return 1;
}

int Trim(char* str)
{
	int len = strlen(str);
	int wpos = strlen(str) - 1;
	int rpos = strlen(str) - 1;
	while (rpos >= 0)
	{
		if (str[rpos] == ' ')
		{
			rpos--;
		}
		else
		{
			if (wpos != rpos)
			{
				str[wpos] = str[rpos];
			}
			rpos--;
			wpos--;
		}
	}
	//清除尾部空格
	//空格数量 = wpos + 1
	int slen = len - (wpos + 1);
	memmove(str, str + wpos + 1, slen);
	memset(str + slen, 0, wpos + 1);
	return wpos;
}

int URLEncode(const char* str, const int strSize, char* result, const int resultSize)
{
	int i;
	int j = 0;//for result index
	char ch;

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0))
	{
		return 0;
	}

	for (i = 0; (i < strSize) && (j < resultSize); ++i)
	{
		ch = str[i];
		if (((ch >= 'A') && (ch < 'Z')) ||
			((ch >= 'a') && (ch < 'z')) ||
			((ch >= '0') && (ch < '9')))
		{
			result[j++] = ch;
		}
		else if (ch == ' ')
		{
			result[j++] = '+';
		}
		else if (ch == '.' || ch == '-' || ch == '_' || ch == '*')
		{
			result[j++] = ch;
		}
		else
		{
			if (j + 3 < resultSize)
			{
				sprintf(result + j, "%%%02X", (unsigned char)ch);
				j += 3;
			}
			else
			{
				return 0;
			}
		}
	}

	result[j] = '\0';
	return j;
}

int URLDecode(const char* str, const int strSize, char* result, const int resultSize)
{
#define NON_NUM '0'
	char ch, ch1, ch2;
	int i;
	int j = 0;//record result index

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0))
	{
		return 0;
	}

	for (i = 0; (i < strSize) && (j < resultSize); ++i)
	{
		ch = str[i];
		switch (ch)
		{
		case '+':
			result[j++] = ' ';
			break;
		case '%':
			if (i + 2 < strSize)
			{
				ch1 = fromHex(str[i + 1]);//高4位
				ch2 = fromHex(str[i + 2]);//低4位
				if ((ch1 != NON_NUM) && (ch2 != NON_NUM))
					result[j++] = (char)((ch1 << 4) | ch2);
				i += 2;
				break;
			}
			else
			{
				break;
			}
		default:
			result[j++] = ch;
			break;
		}
	}

	result[j] = 0;
	return j;
}

void StrUtils::SplitW(const wstring& s, vector<wstring>& sv, const wchar_t flag)
{
	sv.clear();
	wistringstream iss(s);
	wstring temp;

	while (getline(iss, temp, flag))
	{
		sv.push_back(temp);
	}

	return;
}

void StrUtils::SplitA(const std::string& s, vector<std::string>& sv, const char flag)
{
	sv.clear();
	istringstream iss(s);
	std::string temp;

	while (getline(iss, temp, flag))
	{
		sv.push_back(temp);
	}

	return;
}

void StrUtils::Split(const stdstr& s, vector<stdstr>& sv, const TCHAR* str)
{
	sv.clear();
	const TCHAR* p = s.c_str();
	const TCHAR* last = s.c_str();
	int slen = _tcslen(str);
	do
	{
		p = _tcsstr(last, str);
		if (p)
		{
			int len = p - last;
			sv.push_back(stdstr(last, len));
			last += len + slen;
			p += slen;
		}
	} while (p && p - s.c_str() < s.size());
	if (last)
	{
		sv.push_back(last);
	}
}

stdstr StrUtils::Trim(const stdstr& str)
{
	stdstr ret;

	for (auto it = str.begin(); it != str.end(); it++)
	{
		if (*it != _T(' '))
		{
			ret.push_back(*it);
		}
	}
	return ret;
}

stdstr StrUtils::ReplaceAll(const stdstr &str, TCHAR old, TCHAR val)
{
	stdstr ret;
	ret.resize(str.size());
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] != old)
		{
			ret[i] = str[i];
		}
		else
		{
			ret[i] = val;
		}
	}
	return ret;
}

wstring StrUtils::ReplaceAllW(const wstring& str, const wstring&  old_value, const wstring&  new_value)
{
	wstring ret(str);
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = ret.find(old_value)) != string::npos)
			ret.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return ret;
}

string StrUtils::ReplaceAllA(const string& str, const string&  old_value, const string&  new_value)
{
	string ret(str);
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = ret.find(old_value)) != string::npos)
			ret.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return ret;
}
bool StrUtils::EndWith(const std::string& str, const char* model)
{
	size_t nFind = str.find_last_of(model);
	size_t len = strlen(model);
	if (nFind == str.npos)
	{
		return false;
	}
	if (nFind + len == str.length())
	{
		return true;
	}
	return false;
}
std::string StrUtils::UrlDecode(const std::string &SRC)
{
	std::string ret;
	char ch;
	int ii;
	for (size_t i = 0; i < SRC.length(); i++)
	{
		if (int(SRC[i]) == 37)
		{
			sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
		else
		{
			ret += SRC[i];
		}
	}
	return (ret);
}
string StrUtils::UrlEncode(const string& str)
{
	string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			strchr("=!~*'()", str[i]))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "%20";
		else
		{
			strTemp += '%';
			strTemp += toHex((unsigned char)str[i] >> 4);
			strTemp += toHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}
#ifdef _WIN32
std::string StrUtils::ChangeExtend(const std::string& filePath, const char* fileExten)
{
	std::string str(filePath);
	size_t nPos = str.find('.');
	str = str.substr(0, nPos + 1);
	str += fileExten;
	return str;
}
string StrUtils::Toupper(const string& str)
{
	string ret(str.size(), 0);
	for (size_t i = 0; i < str.size(); i++)
	{
		ret[i] = toupper(str[i]);
	}
	return ret;
}
void StrUtils::Format(char* buf, const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	size_t ACTSIZE = _vscprintf(format, argptr);
	buf = (char*)malloc(ACTSIZE + 1);
	memset(buf, 0, ACTSIZE + 1);
	vsnprintf(buf, ACTSIZE + 1, format, argptr);
	va_end(argptr);
}
#endif