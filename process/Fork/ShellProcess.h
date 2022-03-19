#pragma once
#include<Windows.h>
#include "tlhelp32.h"
#include <UserEnv.h>
#include<tchar.h>
#include<memory>
#include<WinBase.h>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"

class ShellProcess
{
public:
	ShellProcess();
	~ShellProcess();
	void SetPath(const stdstr& processPath, const stdstr& command);
	DWORD Wait(DWORD dwMilliseconds = INFINITE);
	DWORD ExitCode();
public:
	BOOL Run();
private:
	SHELLEXECUTEINFO  ShExecInfo = { 0 };
};