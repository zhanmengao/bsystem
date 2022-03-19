#pragma once
#include <iostream>
#include <fstream>
#include<vector>
#include<string>
#include<map>
#define CACHE_FILE "PacketCache"
extern "C"
{
#include"../../../../algorithm/Normal/byd/stb/CString.h"
}
using namespace std;
#define FILE "D:/students.dat"

//in 读方式打开			out 写方式打开
//app 每次写操作前均定位到文件末尾
//ate 打开文件后定位到文件末尾
//trunc 覆盖写（写模式默认）
//binary 字节流
class CStudent
{
public:
	string name;
	int age;
};
static int mainBinaryFileRW()
{
	vector<CStudent> vce;
	CStudent s;
	s.name = "WangBiyu";
	s.age = 25;
	vce.push_back(s);
	s.name = "PengXinyi";
	s.age = 24;
	vce.push_back(s);
	ofstream outFile(FILE, ios::out | ios::binary);
	//while (cin >> s.szName >> s.age)
	for (auto it = vce.begin(); it != vce.end(); it++)
	{
		outFile.write((char*)&(*it), sizeof(*it));
	}

	outFile.close();

	CStudent s_in;
	ifstream inFile(FILE, ios::in | ios::binary); //二进制读方式打开
	if (!inFile)
	{
		cout << "error" << endl;
		return 0;
	}
	//一直读到文件结束
	while (inFile.read((char *)&s_in, sizeof(s_in)))
	{
		int readedBytes = inFile.gcount(); //看刚才读了多少字节
		cout << "Read Size:" << readedBytes << "	" << s_in.name << " " << s_in.age << endl;
	}
	inFile.close();
	return 0;
}

static int mainBinaryGetPut(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "File name missing!" << endl;
		return 0;
	}
	ifstream inFile(argv[1], ios::binary | ios::in);  //以二进制读模式打开文件
	if (!inFile)
	{
		cout << "Source file open error." << endl;
		return 0;
	}
	ofstream outFile(argv[2], ios::binary | ios::out);  //以二进制写模式打开文件
	if (!outFile)
	{
		cout << "New file open error." << endl;
		inFile.close();  //打开的文件一定要关闭
		return 0;
	}
	char c;
	while (inFile.get(c))  //每次读取一个字符
		outFile.put(c);  //每次写入一个字符
	outFile.close();
	inFile.close();
	return 0;
}

struct MQMessage
{
	int tag;
	int target = 0;
	int command;
	cstring str = { 0 };
	MQMessage()
	{
		String_InitSize(&str, 0, '\0');
	}
	~MQMessage()
	{
		String_Free(&str);
	}
	MQMessage(const MQMessage& obj) :target(obj.target), command(obj.command)
	{
		tag = obj.tag;
		String_CopyFrom(&str, &obj.str);
	}
	MQMessage& operator=(const const MQMessage& obj)
	{
		if (*this == obj)
		{
			return *this;
		}
		else
		{
			tag = obj.tag;
			String_CopyFrom(&str, &obj.str);
			target = obj.target;
			command = obj.command;
			return *this;
		}
	}
	bool operator==(const const MQMessage& obj)
	{
		if ((strcmp(str.string, obj.str.string) == 0))
		{
			return true;
		}
		return false;
	}
};

static std::map<int, MQMessage> mCache;
static void ReadCacheFile()
{
	mCache.clear();
	ifstream inFile(CACHE_FILE, ios::in | ios::binary);
	size_t szData = sizeof(MQMessage) - sizeof(cstring);
	size_t strSize = 0;
	while (inFile.read((char *)&strSize, sizeof(strSize)))
	{
		MQMessage iData;
		int readedBytes = inFile.gcount(); //看刚才读了多少字节
		if (strSize <= 1000000)
		{
			String_InitSize(&iData.str, strSize, '\0');
		}
		//读字符串
		inFile.read((char*)(iData.str.string), strSize);
		//读其他
		inFile.read((char*)(&(iData)), szData);

		mCache.insert(std::make_pair(iData.command, iData));
	}
	inFile.close();
}
static void WriteCacheFile()
{
	ofstream outFile(CACHE_FILE, ios::out | ios::binary | ios::trunc);
	size_t szData = sizeof(MQMessage) - sizeof(cstring);
	for (auto it = mCache.begin(); it != mCache.end(); it++)
	{
		//写字符串长度
		outFile.write((char*)(&(it->second.str.nSize)), sizeof(it->second.str.nSize));
		//写字符串
		outFile.write((char*)(it->second.str.string), it->second.str.nSize);
		//写其他
		outFile.write((char*)(&(it->second)), szData);
	}
	outFile.close();
}

static void mainVEIDP()
{
	ReadCacheFile();
	for (int i = 0; i < 10; i++)
	{
		MQMessage msg;
		msg.tag = i;
		msg.target = i + 1;
		msg.command = i + 2;
		String_InitStr(&msg.str, "qaqaqaqaqaq", strlen("qaqaqaqaqaq"));
		mCache.insert(std::make_pair(msg.command, msg));
	}

	WriteCacheFile();
}