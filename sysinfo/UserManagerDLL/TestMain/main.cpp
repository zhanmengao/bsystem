#include"../../../../C++space/Convert/Convert/Convert.h"
#include"Define.h"
#include<stdlib.h>
#include<stdio.h>
#include<json\json.h>
#include<string>
#include<iostream>

using namespace std;
#pragma commect (lib,"UserManager.lib")
#pragma commect (lib,"jsoncpp_static.lib")
#define USER_NAME  "qaq"
#define OLD_PASS  "qaq123456"
#define NEW_PASS  "qaq1234567"
#define NEW_GROUP "gQAQ"
#define _ADD 1
void main()
{
#if _ADD
	char buf[1024] = { 0 };
	int n = 1024;
	cout << ReturnErrInfo(AddUser(USER_NAME, OLD_PASS), buf, n) << buf <<Convert::UTF8_To_string(buf)<< endl;
	
	cout << ReturnErrInfo(SetUserPasswordAdmin(USER_NAME, NEW_PASS), buf, n) << buf << endl;
	cout << ReturnErrInfo(AddGroup(NEW_GROUP), buf, n) << buf << endl;
	cout << ReturnErrInfo(AddUserToGroup(NEW_GROUP, USER_NAME), buf, n) << buf << endl;
	cout << ReturnErrInfo(DelUserToGroup(NEW_GROUP, USER_NAME), buf, n) << buf << endl;
	cout << ReturnErrInfo(AddUserToGroup(NEW_GROUP, USER_NAME), buf, n) << buf << endl;
	cout << ReturnErrInfo(SetUserActive(USER_NAME, false), buf, n) << buf << endl;
#endif
	cout << ReturnErrInfo(AddUserPlus(USER_NAME, OLD_PASS, NEW_GROUP,nullptr), buf, n) << Convert::Utf8ToASCII(string(buf)) << endl;
	//²âÊÔÁÐ¾Ù
	char gName[1024] = { 0 };
	int gSize = 0;
	char uName[1024] = { 0 };
	int uSize = 0;
	char ugName[1024] = { 0 };
	int ugSize = 0;
	Json::Reader reader;
	Json::Value root;
	Json::Value data;

	Json::Reader uReader;
	Json::Value uRoot;
	Json::Value uData;

	Json::Reader ugReader;
	Json::Value ugRoot;
	Json::Value ugData;

	cout << "GetGroupLst<"<<ReturnErrInfo(GetGroupList(gName, gSize),buf,n) << Convert::Utf8ToASCII(string(buf))<< endl;
	cout << "GetGroupLst<" << gName<<gSize << endl;
	if (reader.parse(gName, root))
	{
		data = root["Data"];
		for (unsigned i = 0; i < data.size(); i++)
		{
			std::string groupName = data[i].asString();
			std::cout << "Group:" << groupName << std::endl;
			GetGroupUsers(groupName.c_str(), uName, uSize);
			if (uReader.parse(uName, uRoot))
			{
				uData = uRoot["Data"];
				for (unsigned j = 0; j < uData.size(); j++)
				{
					std::string userName = uData[j].asString();
					std::cout << "User:	" << userName << std::endl;
					GetUserGroups(userName.c_str(), ugName, ugSize);
					if (ugReader.parse(ugName, ugRoot))
					{
						ugData = ugRoot["Data"];
						for (unsigned k = 0; k < ugData.size(); k++)
						{
							std::string usgName = ugData[k].asString();
							std::cout << "Group:		" << usgName << std::endl;
						}
					}
				}
			}
		}
	}

	/*	printf("GetGroupUsers %s\n", ReturnErrInfo(GetGroupUsers(GROUP, uName, uSize)));
		printf("GetGroupUsers val: %s nSize: %u \n", uName, uSize);
		if (reader.parse(uName, root))
		{
			Json::Value data = root["Data"];
			std::string out = data.toStyledString();
			printf("Data:%s :\n", out.c_str());
			for (unsigned i = 0; i < data.size(); i++)
			{
				std::cout << data[i].asString() << std::endl;
			}
		}

		printf("GetUserGroups %s\n", ReturnErrInfo(GetUserGroups(USER_NAME, ugName, ugSize)));
		printf("GetUserGroups val: %s ugSize: %u\n", ugName, ugSize);
		if (reader.parse(ugName, root))
		{
			Json::Value data = root["Data"];
			std::string out = data.toStyledString();
			printf("Data:%s :\n", out.c_str());
			for (unsigned i = 0; i < data.size(); i++)
			{
				std::cout << data[i].asString() << std::endl;
			}
		}
		*/
#if _ADD
	cout << ReturnErrInfo(SetUserActive(USER_NAME, true), buf, n) << buf << endl;
	cout << ReturnErrInfo(DelGroup(NEW_GROUP), buf, n) << buf << endl;
	cout << ReturnErrInfo(SetUserPassword(USER_NAME, NEW_PASS, OLD_PASS), buf, n) << buf << endl;
	//cout << ReturnErrInfo(DelUser(USER_NAME), buf, n) << buf << endl;
#endif
	system("pause");
}