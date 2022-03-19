#pragma once

#define _WINSOCKAPI_
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
using namespace std;

class Tools
{
public:
	static bool SaveDataToFile(const std::string& szFilePath, const std::string& text);
	static wstring GetProcessNameByPid(DWORD nPid);
	static wstring GetWindowTitle(HWND hwnd);

	static _SYSTEMTIME GetTime();
	static string GetFormatTime();
	static string GetFormatTime(const _SYSTEMTIME& t);
	static string GetCurDir();
	static string GetModuleFolder();

	static bool Shift();
	static bool Ctrl();
	static bool Alt();
	static bool CapLock();
	static bool IsSystem();
	static bool IsRunasAdmin();
public:
	//convert
	static wstring string2wstring(const string& str);
	static string wstring2string(const wstring& str);
	static string GetKeyName(unsigned int vkCode);
protected:
private:
	static ofstream mLogFileStream;
};