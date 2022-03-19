#include"RemoteApp.h"
#include"Apps.h"
#include<iostream>
void main()
{
	{
		vector<RegApplication> vec;
		AppManager::GetSoftListInfo(vec);
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			cout << Convert::UnicodeToASCII(it->DisplayName) << endl;
		}
	}
	cout << endl;
	{
		set<Application> s;
		AppManager::GetInstallAppList(s);
		for (auto it = s.begin(); it != s.end(); it++)
		{
			std::cout << Convert::UnicodeToASCII(it->mName) << "	" << Convert::UnicodeToASCII(it->mPath) << endl;
		}
	}

	RemoteApp app;
	app.RegName = L"QAQ";
	printf("AppendRemoteApp : %u \n", RemoteAppUtils::AppendRemoteApp(app));

	vector<RemoteApp> appList;
	printf("GetRemoteAppList : %u \n", RemoteAppUtils::GetRemoteAppList(appList));
	for (auto it = appList.begin(); it != appList.end(); it++)
	{
		printf("name %ws \n", it->RegName.c_str());
	}
	printf("DeleteRemoteApp : %u \n", RemoteAppUtils::DeleteRemoteApp(app.RegName.c_str()));
	system("pause");
}