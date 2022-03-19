#ifndef _STRUTILS_H
#define _STRUTILS_H
#include"StringHead.h"
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include"../Convert/CppConvert.h"
#ifdef __cplusplus
#include<string>
#include<vector>
#include<sstream>
#ifdef _WIN32
#include"../Convert/Convert.h"
#include<tchar.h>
#endif
#include<iostream>
#endif


int Find(const char* haystack, const char* needle);
unsigned Count(const char* haystack, size_t nSize, const char* needle);
int replaceAll(char* string, size_t bufferSize, const char* src, const char* target,
	size_t* outSize);

int Trim(char* str);
int URLEncode(const char* str, const int strSize, char* result, const int resultSize);

int URLDecode(const char* str, const int strSize, char* result, const int resultSize);




#ifdef __cplusplus
class StrUtils
{
public:
#if _UNICODE
#define Split SplitW
#else
#define Split SplitA
#endif
	static void SplitW(const wstring& s, vector<wstring>& sv, const wchar_t flag = ' ');
	static void SplitA(const std::string& s, vector<std::string>& sv, const char flag = ' ');
	static void Split(const stdstr& s, vector<stdstr>& sv, const TCHAR* str = _T(""));
	static stdstr Trim(const stdstr& str);
	static stdstr ReplaceAll(const stdstr &str, TCHAR old, TCHAR val);
	static wstring ReplaceAllW(const wstring& str, const wstring&  old_value, const wstring&  new_value);
	static string ReplaceAllA(const string& str, const string&  old_value, const string&  new_value);
#ifdef _WIN32
	static void Format(char* buf, const char* format, ...);
	static string Toupper(const string& str);
	//修改拓展名
	static std::string ChangeExtend(const std::string& filePath, const char* fileExten);
#endif
	static string UrlEncode(const string& str);

	static std::string UrlDecode(const std::string &SRC);
	static bool EndWith(const std::string& str, const char* model);
};
#endif
#endif
/*
1	void *memchr(const void *str, int c, size_t n)
在参数 str 所指向的字符串的前 n 个字节中搜索第一次出现字符 c（一个无符号字符）的位置。

2	int memcmp(const void *str1, const void *str2, size_t n)
把 str1 和 str2 的前 n 个字节进行比较。

3	void *memcpy(void *dest, const void *src, size_t n)
从 src 复制 n 个字符到 dest。

4	void *memmove(void *dest, const void *src, size_t n)
另一个用于从 src 复制 n 个字符到 dest 的函数。

5	void *memset(void *str, int c, size_t n)
复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。

6	char *strcat(char *dest, const char *src)
把 src 所指向的字符串追加到 dest 所指向的字符串的结尾。

7	char *strncat(char *dest, const char *src, size_t n)
把 src 所指向的字符串追加到 dest 所指向的字符串的结尾，直到 n 字符长度为止。

8	char *strchr(const char *str, int c)
在参数 str 所指向的字符串中搜索第一次出现字符 c（一个无符号字符）的位置。

9	int strcmp(const char *str1, const char *str2)
把 str1 所指向的字符串和 str2 所指向的字符串进行比较。

10	int strncmp(const char *str1, const char *str2, size_t n)
把 str1 和 str2 进行比较，最多比较前 n 个字节。

11	int strcoll(const char *str1, const char *str2)
把 str1 和 str2 进行比较，结果取决于 LC_COLLATE 的位置设置。

12	char *strcpy(char *dest, const char *src)
把 src 所指向的字符串复制到 dest。

13	char *strncpy(char *dest, const char *src, size_t n)
把 src 所指向的字符串复制到 dest，最多复制 n 个字符。

14	size_t strcspn(const char *str1, const char *str2)
检索字符串 str1 开头连续有几个字符都不含字符串 str2 中的字符。

15	char *strerror(int errnum)
从内部数组中搜索错误号 errnum，并返回一个指向错误消息字符串的指针。

16	size_t strlen(const char *str)
计算字符串 str 的长度，直到空结束字符，但不包括空结束字符。

17	char *strpbrk(const char *str1, const char *str2)
检索字符串 str1 中第一个匹配字符串 str2 中字符的字符，不包含空结束字符。也就是说，依次检验字符串 str1 中的字符，当被检验字符在字符串 str2 中也包含时，则停止检验，并返回该字符位置。

18	char *strrchr(const char *str, int c)
在参数 str 所指向的字符串中搜索最后一次出现字符 c（一个无符号字符）的位置。

19	size_t strspn(const char *str1, const char *str2)
检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。

20	char *strstr(const char *haystack, const char *needle)
在字符串 haystack 中查找第一次出现字符串 needle（不包含空结束字符）的位置。

21	char *strtok(char *str, const char *delim)
分解字符串 str 为一组字符串，delim 为分隔符。

22	size_t strxfrm(char *dest, const char *src, size_t n)
根据程序当前的区域选项中的 LC_COLLATE 来转换字符串 src 的前 n 个字符，并把它们放置在字符串 dest 中。
*/
