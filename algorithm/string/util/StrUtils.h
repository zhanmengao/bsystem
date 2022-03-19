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
	//�޸���չ��
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
�ڲ��� str ��ָ����ַ�����ǰ n ���ֽ���������һ�γ����ַ� c��һ���޷����ַ�����λ�á�

2	int memcmp(const void *str1, const void *str2, size_t n)
�� str1 �� str2 ��ǰ n ���ֽڽ��бȽϡ�

3	void *memcpy(void *dest, const void *src, size_t n)
�� src ���� n ���ַ��� dest��

4	void *memmove(void *dest, const void *src, size_t n)
��һ�����ڴ� src ���� n ���ַ��� dest �ĺ�����

5	void *memset(void *str, int c, size_t n)
�����ַ� c��һ���޷����ַ��������� str ��ָ����ַ�����ǰ n ���ַ���

6	char *strcat(char *dest, const char *src)
�� src ��ָ����ַ���׷�ӵ� dest ��ָ����ַ����Ľ�β��

7	char *strncat(char *dest, const char *src, size_t n)
�� src ��ָ����ַ���׷�ӵ� dest ��ָ����ַ����Ľ�β��ֱ�� n �ַ�����Ϊֹ��

8	char *strchr(const char *str, int c)
�ڲ��� str ��ָ����ַ�����������һ�γ����ַ� c��һ���޷����ַ�����λ�á�

9	int strcmp(const char *str1, const char *str2)
�� str1 ��ָ����ַ����� str2 ��ָ����ַ������бȽϡ�

10	int strncmp(const char *str1, const char *str2, size_t n)
�� str1 �� str2 ���бȽϣ����Ƚ�ǰ n ���ֽڡ�

11	int strcoll(const char *str1, const char *str2)
�� str1 �� str2 ���бȽϣ����ȡ���� LC_COLLATE ��λ�����á�

12	char *strcpy(char *dest, const char *src)
�� src ��ָ����ַ������Ƶ� dest��

13	char *strncpy(char *dest, const char *src, size_t n)
�� src ��ָ����ַ������Ƶ� dest����ิ�� n ���ַ���

14	size_t strcspn(const char *str1, const char *str2)
�����ַ��� str1 ��ͷ�����м����ַ��������ַ��� str2 �е��ַ���

15	char *strerror(int errnum)
���ڲ���������������� errnum��������һ��ָ�������Ϣ�ַ�����ָ�롣

16	size_t strlen(const char *str)
�����ַ��� str �ĳ��ȣ�ֱ���ս����ַ������������ս����ַ���

17	char *strpbrk(const char *str1, const char *str2)
�����ַ��� str1 �е�һ��ƥ���ַ��� str2 ���ַ����ַ����������ս����ַ���Ҳ����˵�����μ����ַ��� str1 �е��ַ������������ַ����ַ��� str2 ��Ҳ����ʱ����ֹͣ���飬�����ظ��ַ�λ�á�

18	char *strrchr(const char *str, int c)
�ڲ��� str ��ָ����ַ������������һ�γ����ַ� c��һ���޷����ַ�����λ�á�

19	size_t strspn(const char *str1, const char *str2)
�����ַ��� str1 �е�һ�������ַ��� str2 �г��ֵ��ַ��±ꡣ

20	char *strstr(const char *haystack, const char *needle)
���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle���������ս����ַ�����λ�á�

21	char *strtok(char *str, const char *delim)
�ֽ��ַ��� str Ϊһ���ַ�����delim Ϊ�ָ�����

22	size_t strxfrm(char *dest, const char *src, size_t n)
���ݳ���ǰ������ѡ���е� LC_COLLATE ��ת���ַ��� src ��ǰ n ���ַ����������Ƿ������ַ��� dest �С�
*/
