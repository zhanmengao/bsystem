#pragma once
#include<vector>
#include<tchar.h>
#include<Windows.h>
using namespace std;
static const TCHAR* GetCommand()
{
	return GetCommandLine();
}
vector<wstring> GetCommandArray()
{
	vector<wstring> strlist;
	int nSize = 0;
	wchar_t** command = CommandLineToArgvW(GetCommandLineW(), &nSize);
	for (int i = 0; i < nSize; i++)
	{
		strlist.push_back(command[i]);
	}
	HeapFree(GetProcessHeap(), 0, command);
	return strlist;
}