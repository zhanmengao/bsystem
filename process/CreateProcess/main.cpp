#include<stdio.h>
#include<stdlib.h>
#include"ProcessMonitList.h"
#include"ProcessMonitWMI.h"
#include<iostream>
#include"ProcessElevationUtils.h"
using namespace std;

unsigned __stdcall Thread(void* argv)
{
	ProcessMonitList* monitlist = static_cast<ProcessMonitList*>(argv);
	if (monitlist)
	{
		monitlist->MonitProcess(1);
	}
	_endthreadex(0);
	return 0;
}
unsigned __stdcall ThreadWMI(void* argv)
{
	ProcessMonitWMI* monitlist = static_cast<ProcessMonitWMI*>(argv);
	if (monitlist)
	{
		monitlist->MonitProcess(1);
	}
	_endthreadex(0);
	return 0;
}
void mainMonit()
{
	ProcessMonitList monitlist;
	monitlist.SetTimer(500);
	//_beginthreadex(NULL, 0, Thread, &monitlist, NULL, NULL);

	//ProcessMonitWMI monitWMI;
	//monitWMI.SetTimer(100);
	//_beginthreadex(NULL, 0, ThreadWMI, &monitWMI, NULL, NULL);
	
	system("pause");
	monitlist.SetUpdate(false);
	//monitWMI.SetUpdate(false);
	cout << "--------------------------------------------List--------------------------------------------" << endl;
	std::map<stdstr, DWORD> countMap = monitlist.GetCountMap();
	for (auto it = countMap.begin(); it != countMap.end(); it++)
	{
		wcout << it->first << "	" << it->second << endl;
	}
	cout << "--------------------------------------------WMI--------------------------------------------" << endl;
	//countMap = monitWMI.GetCountMap();
	for (auto it = countMap.begin(); it != countMap.end(); it++)
	{
		wcout << it->first << "	" << it->second << endl;
	}
}

void main()
{
	std::map<DWORD, ProcessInfo> mapProcess;
	ProcessHelper::GetProcessList(mapProcess);
	for (auto it = mapProcess.begin(); it != mapProcess.end(); it++)
	{
		wprintf(L"pid % u,szExeFile: %s \n", it->first, it->second.szExeFile.c_str());
	}

	auto timesInfo = ProcessHelper::GetProcessTime(ProcessHelper::GetProcess());
	cout << timesInfo.KernelTime.dwLowDateTime << endl;
	auto ioInfo = ProcessHelper::GetIOInfo(ProcessHelper::GetProcess());
	cout << ioInfo.OtherOperationCount << endl;

	ProcessTree tree;
	ProcessHelper::GetProcessTreefromParentID(mapProcess, 8184, tree);
	system("pause");

	ProcessElevationUtils::UACWhiteList(_T("C:\\Windows\\System32\\cmd.exe"));
}