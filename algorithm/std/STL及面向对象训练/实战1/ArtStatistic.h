#pragma once
#include<iostream>
#include <string>
#include <fstream>
#include<map>
#include <vector>
#include <algorithm>
using namespace std;
//////////////////////////////////////////////////////////////////////////
//Ӣ�������ļ�ͳ����
//////////////////////////////////////////////////////////////////////////
class ArtStatistic
{
public:
	ArtStatistic();
	~ArtStatistic();

	//�ļ�����
	bool openFile(string filePath);
	void closeFile();
	
	void statistic();	//ͳ������
	void outDatas();	//�������
private:
	void addWord(string &word);//��ʱ������һ������
	string &toLowerString(string &str) //ȫ��ת��ΪСд
	{
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		return str;
	}
	fstream _fs;	//�ļ�������
	map<string,int> _words;	//ͳ������
};
//////////////////////////////////////////////////////////////////////////
//�и��ַ���������
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
	void reset()  //���ò���λ�õ���һ������
	{
		this->_pos = 0;
	}
	bool eos()	//End Of String
	{
		return this->_pos >= this->_words.size() ? true : false;
	}
private:
	void split()  //�з�
	{
		static char buf[64];
		memset(buf, '\0', 64);
		const char *pstr = this->_buf.c_str();
		const char* pstrHead = pstr, *pstrTail = pstr;
		while (*pstrTail != '\0' )
		{
			if ( canSplit(*pstrTail) )  //�������ſ�ʼ��ȡ����
			{
				if (pstrHead < pstrTail) //���ǵ���������
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

	bool canSplit(char ch)//����ַ��Ƿ�Ϊ��㣨��������ӷ��ţ�-��_��
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

