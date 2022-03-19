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

//in ����ʽ��			out д��ʽ��
//app ÿ��д����ǰ����λ���ļ�ĩβ
//ate ���ļ���λ���ļ�ĩβ
//trunc ����д��дģʽĬ�ϣ�
//binary �ֽ���
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
	ifstream inFile(FILE, ios::in | ios::binary); //�����ƶ���ʽ��
	if (!inFile)
	{
		cout << "error" << endl;
		return 0;
	}
	//һֱ�����ļ�����
	while (inFile.read((char *)&s_in, sizeof(s_in)))
	{
		int readedBytes = inFile.gcount(); //���ղŶ��˶����ֽ�
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
	ifstream inFile(argv[1], ios::binary | ios::in);  //�Զ����ƶ�ģʽ���ļ�
	if (!inFile)
	{
		cout << "Source file open error." << endl;
		return 0;
	}
	ofstream outFile(argv[2], ios::binary | ios::out);  //�Զ�����дģʽ���ļ�
	if (!outFile)
	{
		cout << "New file open error." << endl;
		inFile.close();  //�򿪵��ļ�һ��Ҫ�ر�
		return 0;
	}
	char c;
	while (inFile.get(c))  //ÿ�ζ�ȡһ���ַ�
		outFile.put(c);  //ÿ��д��һ���ַ�
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
		int readedBytes = inFile.gcount(); //���ղŶ��˶����ֽ�
		if (strSize <= 1000000)
		{
			String_InitSize(&iData.str, strSize, '\0');
		}
		//���ַ���
		inFile.read((char*)(iData.str.string), strSize);
		//������
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
		//д�ַ�������
		outFile.write((char*)(&(it->second.str.nSize)), sizeof(it->second.str.nSize));
		//д�ַ���
		outFile.write((char*)(it->second.str.string), it->second.str.nSize);
		//д����
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