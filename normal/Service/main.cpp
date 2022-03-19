#include "Service.h"
#include"ServiceCtrl.h"
/**
* @brief main Function
*/
int main(int argc, char* argv[])
{
	vector<_ServicesInfo> vec;
	ServiceCtrl::GetServiceList(vec);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		STDOUT << it->serviceName <<endl;
	}
	return 0;
	SERVICE_TABLE_ENTRY svTable[] = 
	{
		{ L"BackDoor",ServiceMain2 },
		{ NULL,NULL }
	};
	StartServiceCtrlDispatcher(svTable);
	if (InstallService() == SERVICE_OP_ERROR)
	{
		printf("[!]Service Operation Error\n");
	}
	else
	{
		printf("[*]Service Operation Success\n");
	}
	return 0;
}

void DoTask()
{
	ShowMessage(L"Hi Demon·Gan\nThis Is From Session 0 Service!\n", L"HELLO");
	// 创建用户桌面进程
	//CreateUserProcess(L"D:\\Space\\Safe\\KeyWordConsole\\Debug\\KeyWordConsole.exe");
}