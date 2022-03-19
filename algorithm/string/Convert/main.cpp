#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include"Convert.h"
#include<iostream>
using namespace std;

//找到char数组里的中文
string GetChinese(const char* str)			//
{
	string ret;
	unsigned len = strlen(str);
	for (int i = 0; i < len;i++)
	{
		if (str[i] < 0)						//负数则前后两个字节存的是汉字
		{
			ret.push_back(str[i++]);
			ret.push_back(str[i]);
		}
	}
	return ret;
}
wstring GetChinese(const WCHAR* str)			//
{
	wstring ret;
	unsigned len = wcslen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] >>7 )						//判断高字节是否为0，如果不是，则为汉字
		{
			ret.push_back(str[i++]);
			ret.push_back(str[i]);
		}
	}
	return ret;
}
int main()
{
	int a = 217;
	char b;

	string data("我只是");     //ansi

	string utf8("\"\\u0392\\u011c\\u024f\\u0685\\u00b0\"\n");
	string sutf8 = Convert::string_To_UTF8(data);    //utf8
	cout <<"sutf8:"<<sutf8 << endl;

	string sansi = Convert::UTF8_To_string(sutf8);       //ansi
	cout <<"sansi:"<< sansi << endl;

	wstring ws = Convert::Utf8ToUnicode(utf8.c_str());    //utf8
	wcout << "utf8:" << ws << endl;

	ws = Convert::StringToWString(data);      //ws
	wcout <<L"ws:"<<ws << endl;

	string s = Convert::WStringToString(ws);    //ansi
	cout << "s:" << s << endl;

	return 0;
}
