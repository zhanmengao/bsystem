#include"../../WindowsProcessHelper/Fork/ChildProcess.h"
#include<vector>
#include<tchar.h>
#if _DEBUG
#else
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
void main()
{
	int argc = 0;
	wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc < 2)
	{
		return;
	}
	std::vector<OSProcess*> processList;
	for (int i = 1; i < argc; i++)
	{
		std::wstring path = argv[i];
		OSProcess *process = new OSProcess();

		process->SetStartPath(path, path);
		process->SetStartShowWindow(true);
		process->Start();
		processList.push_back(process);
	}
#ifdef _DEBUG
	system("pause");
	for (auto it = processList.begin(); it != processList.end(); it++)
	{
		(*it)->Exit();
		delete *it;
	}
#else
	for (auto it = processList.begin(); it != processList.end(); it++)
	{
		(*it)->Wait();
		delete *it;
	}
#endif
}