#include"../../../../Network/Serialization/Json/JsonTools.hpp"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include<json\json.h>

#include<iostream>
#include"../AppManager/RemoteApp.h"
#include"AppManager.h"
void main()
{
	printf("InsertRemoteApp %u \n",InsertRemoteApp("QAQ", NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL));
	printf("SetShowName %u \n", SetShowName("QAQ","QQQ"));
	printf("SetPath %u \n", SetPath("QAQ", "PATH"));
	printf("SetIconIndex %u \n", SetIconIndex("QAQ", 1));
	printf("SetIconPath %u \n", SetIconPath("QAQ", "ICONPATH"));
	printf("SetCommandLineSetting %u \n", SetCommandLineSetting("QAQ", 1));
	printf("SetRequiredCommandLine %u \n", SetRequiredCommandLine("QAQ", "REQ"));
	printf("SetShowInTSWA %u \n", SetShowInTSWA("QAQ", 2));
	printf("SetVPath %u \n", SetVPath("QAQ", "VPath"));
	printf("SetSecurityDescriptor %u \n", SetSecurityDescriptor("QAQ", "SD"));
	char buf[40960] = { 0 };
	int len = 0;
	GetRemoteAppList(buf, len);
	Json::Value root;
	JsonTools::SetJsonData(buf, root);
	Json::Value remoteApp = root[REMOTEAPP_LIST];
	for (int i = 0; i < remoteApp.size(); i++)
	{
		//std::wcout << Convert::Utf8ToUnicode(remoteApp[i][REG_NAME_KEY].asString()) << std::endl;
	}

	GetAppList(buf, len);
	JsonTools::SetJsonData(buf, root);
	Json::Value appList = root[APP_LIST];
	for (int i = 0; i < appList.size(); i++)
	{
		//std::cout << Convert::UnicodeToASCII(appList[i][REG_NAME_KEY].asString()) << std::endl;
	}

	//printf("DeleteRemoteApp %u \n", DeleteRemoteApp("QAQ"));
	system("pause");
}