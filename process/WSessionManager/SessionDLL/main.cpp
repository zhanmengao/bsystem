#include"../../../../C++space/Convert/Convert/Convert.h"
#include"SessionManager.h"
#include<stdio.h>
#include<iostream>
#include".. /../../../../../Network/Serialization/Json/JsonTools.hpp"
#pragma comment (lib,"jsoncpp_static.lib")
#include<json\json.h>
void main()
{
	printf("PID =  %lu \n", GetCurrentProcessId());
	system("pause");
	char buf[40960] = { 0 };
	int len = 0;
	printf("%u %s\n", GetSessionList(buf, len), buf);

	Json::Value root;
	JsonTools::SetJsonData(buf, root);
	Json::Value SessionList = root[SESSION_LIST];
	for (int i = 0; i < SessionList.size(); i++)
	{
		std::wcout << Convert::Utf8ToUnicode(SessionList[i]["SName"].asString()) << std::endl;
		DWORD sid = atoi(SessionList[i]["SID"].asString().c_str());
		//获取进程列表
		DWORD ret = GetProcessList(sid, buf, len);
		printf("	GetProcessList %u %s \n", ret,buf);
		Json::Value proroot;
		JsonTools::SetJsonData(buf, proroot);
		Json::Value ProList = proroot[PROCESS_LIST];
		for (int j = 0; j < ProList.size(); j++)
		{
			DWORD pid = atoi(ProList[j]["PID"].asString().c_str());
			std::cout << pid << "	";
			wstring str = Convert::Utf8ToUnicode(ProList[j]["PName"].asString());
			if (str.empty())
			{
				std::cout << endl;
			}
			if (str == L"notepad++.exe")
			{
				printf("KillProcess %u \n", KillProcess(pid));
			}
			std::wcout << str;

			str = Convert::Utf8ToUnicode(ProList[j]["ExeFile"].asString());
			std::wcout <<"		Exe:"<<str;
			std::wcout << endl;
		}
		if (sid != 0)
		{
			printf("CreateEXE %u \n", CreateEXE(sid, "E:\\Notepad++\\notepad++.exe"));
			ULONG res;
			printf("SendMsg %u \n", SendMsg(sid, Convert::PChar2GolangBuf("QAQ搜索").c_str(), 
				Convert::PChar2GolangBuf("我是中文").c_str(), res));
		}

		cout << "Remote App-----------------------------------------------" << endl;
		GetRemoteAPPList(sid, buf, len);
		JsonTools::SetJsonData(buf, proroot);
		ProList = proroot[PROCESS_LIST];
		for (int j = 0; j < ProList.size(); j++)
		{
			std::cout << ProList[j]["PID"].asString() << "	";
			wstring str = Convert::Utf8ToUnicode(ProList[j]["PName"].asString());
			if (str.empty())
			{
				std::cout << endl;
			}
			std::wcout << str << std::endl;
		}
	}
}