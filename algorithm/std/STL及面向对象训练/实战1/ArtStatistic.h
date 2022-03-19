#pragma once
#include<iostream>
#include <string>
#include <fstream>
#include<map>
#include <vector>
#include <algorithm>
using namespace std;
//////////////////////////////////////////////////////////////////////////
//英文文章文件统计类
//////////////////////////////////////////////////////////////////////////
class ArtStatistic
{
public:
	ArtStatistic();
	~ArtStatistic();

	//文件操作
	bool openFile(string filePath);
	void closeFile();
	
	void statistic();	//统计数据
	void outDatas();	//输出数据
private:
	void addWord(string &word);//向词表中添加一个单词
	string &toLowerString(string &str) //全部转换为小写
	{
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		return str;
	}
	fstream _fs;	//文件流操作
	map<string,int> _words;	//统计数据
};
//////////////////////////////////////////////////////////////////////////
//切割字符串辅助类
//////////////////////////////////////////////////////////////////////////
class Spliter
{
public:
	Spliter(char* str) :_pos(0), _buf(str){ this->split(); }
	Spliter(string &str) :_pos(0), _buf(str){ this->split(); }
	~Spliter(){}
	string getWord()
	{
		return this->_words[_pos++];
	}
	void reset()  //重置操作位置到第一个单词
	{
		this->_pos = 0;
	}
	bool eos()	//End Of String
	{
		return this->_pos >= this->_words.size() ? true : false;
	}
private:
	void split()  //切分
	{
		static char buf[64];
		memset(buf, '\0', 64);
		const char *pstr = this->_buf.c_str();
		const char* pstrHead = pstr, *pstrTail = pstr;
		while (*pstrTail != '\0' )
		{
			if ( canSplit(*pstrTail) )  //遇到符号开始提取单词
			{
				if (pstrHead < pstrTail) //不是的连续符号
				{
					_memccpy(buf, pstrHead, sizeof(char), pstrTail - pstrHead);
					this->_words.push_back(buf);
					memset(buf, '\0', 64);
				}
				pstrHead = pstrTail;
				pstrHead++;
			}
			++pstrTail;
		}
	}

	bool canSplit(char ch)//检测字符是否为标点（不检测连接符号：-和_）
	{
		static const char* splitFlags = " ,;.~`!@#$%^&*()-_+=[]{}:;\"'|\\/?";
		for (size_t i = 0; i < strlen(splitFlags);i++)
		{
			if (ch == splitFlags[i])
			{
				return true;
			}
		}
		return false;
	}
	string _buf;
	vector<string> _words;
	unsigned int _pos;
};

