#include"../../WindowsProcessHelper/CreateProcess/SessionManager.h"

void mainAll()
{
	vector<RDPLoginSessionInfo> vec;
	SessionManager::GetWTSSessions(vec);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		printf("Sid % u,User Name: %s \n", it->sid, it->User.WTSUserName.c_str());
		map<DWORD, WTSProcessInfo> proMap;
		SessionManager::GetWTSProcesses(it->sid, proMap);
		for (auto mit = proMap.begin(); mit != proMap.end(); mit++)
		{
			printf("	pid %u , pProcessName : %s ", mit->first, mit->second.pProcessName.c_str());
			printf("\n");
		}
	}
}

void mainRDPInit()
{
	vector<RDPLoginSessionInfo> vec;
	SessionManager::GetWTSSessions(vec);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << "------------------------------SID = " << it->sid << "------------------------------------"<<endl;
		map<DWORD, WTSProcessInfo> proMap;
		BOOL bRet = SessionManager::GetWTSProcesses(it->sid, proMap);
		if (!bRet)
		{
			printf("GetWTSProcesses Error %d \n", GetLastError());
		}
		std::map<DWORD, ProcessInfo> mapProcess;
		ProcessHelper::GetProcessList(mapProcess);
		for (auto it = proMap.begin(); it != proMap.end(); it++)
		{
			printf("pid = %ld name = %s ppid = %ld \n", it->first, it->second.pProcessName.c_str(), ProcessHelper::GetParentID(it->first));
			vector<ProcessInfo> vec;
			ProcessHelper::GetProcessfromParentID(mapProcess, it->first, vec);
			for (int i = 0; i < vec.size(); i++)
			{
				auto& info = vec[i];
				printf("	child[%d] pid = %ld name = %s ppid = %ld \n", i, info.th32ProcessID, info.szExeFile.c_str(), info.th32ParentProcessID);
			}
		}
		cout << endl;
	}
	getchar();
}
void main()
{
	mainRDPInit();
}
