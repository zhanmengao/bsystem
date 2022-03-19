#pragma once
#include"../../../Setting/TableManager/TableManager.h"
#include"../../WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../WindowsProcessHelper/Fork/ChildProcess.h"
#include"../../../IPC/WindowsPipe/PipeServer/Pipe/IPCAnonManager.h"
#include"../../../../C++space/Convert/Convert/CppConvert.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Error/WErrorUtils/ErrorInfo.h"
#include<thread>
struct ScriptItem
{
	stdstr appName;
	stdstr Script;
	stdstr Command;
	bool bRestart;
};
static DWORD CommandCall(const ScriptItem& item, PipeEvent e,void* argv = nullptr)
{
	DWORD dwRet = 0;
	ChildProcess process;
	IPCAnon* pipe = new IPCAnon();
	HANDLE hRead, hWrite;
	pipe->Init(hRead, hWrite, e, argv);
	bool bRet = process.InitProcess(item.Script, item.Command, hRead, hWrite);
	if (!bRet)
	{
		dwRet = GetLastError();
		TCHAR str[1024] = { 0 };
		size_t size = _countof(str);
		ErrorMessage(dwRet, str, size);
		TCHAR buffer[1024] = { 0 };
		wsprintf(buffer, _T("%s %s"), item.Script.c_str(), str);
		if (e)
		{
#ifdef _UNICODE
			e(Convert::WStringToString(buffer), argv);
#else
			e(buffer, argv);
#endif
		}
	}
	else
	{
		process.Wait();
		dwRet = process.ExitCode();
	}
	std::string rData;
	if (pipe)
	{
		pipe->Destroy(&rData);
		delete pipe;
		pipe = nullptr;
	}
	if (e)
	{
		e(rData, argv);
	}	
	return dwRet;
}

static DWORD ShellCall(const ScriptItem& item, PipeEvent e, void* argv = nullptr)
{
	DWORD dwRet = 0;
	IPCAnon* pipe = new IPCAnon();
	HANDLE hRead, hWrite;
	pipe->Init(hRead, hWrite, e, argv);
	BOOL bRet = ForkUtils::ShellExec(_T("powershell"), _T("D:\\Space\\System\\Process\\Install\\Install\\test.ps1"),true);
	if (!bRet)
	{
		dwRet = GetLastError();
		TCHAR str[1024] = { 0 };
		size_t size = _countof(str);
		ErrorMessage(dwRet, str, size);
		TCHAR buffer[1024] = { 0 };
		wsprintf(buffer, _T("%s %s"), item.Script.c_str(), str);
		if (e)
		{
#ifdef _UNICODE
			e(Convert::WStringToString(buffer), argv);
#else
			e(buffer, argv);
#endif
		}
	}
	else
	{
	}
	std::string rData;
	if (pipe)
	{
		pipe->Destroy(&rData);
		delete pipe;
		pipe = nullptr;
	}
	if (e)
	{
		e(rData, argv);
	}
	return dwRet;
}

static void CommandCall(const char* tableName, PipeEvent e)
{
	TableManager tabManager;
	tabManager.Load(tableName);
	const DATA_MAP &tab = tabManager.GetData();
	for (auto it = tab.cbegin(); it != tab.cend(); it++)
	{
		stdstr script;
		stdstr command;
		bool restart = false;
		auto fit = it->second.find(_T("Script"));
		if (fit != it->second.end())
		{
			script = fit->second;
		}
		fit = it->second.find(_T("Command"));
		if (fit != it->second.end())
		{
			command = fit->second;
		}

		fit = it->second.find(_T("Restart"));
		if (fit != it->second.end())
		{
#ifdef _UNICODE
			restart = atoi(CppConvert::UnicodeToANSI(fit->second).c_str());
#else
			restart = atoi(fit->second.c_str());
#endif
		}
		ScriptItem item;
		item.bRestart = restart;
		item.Script = script;
		item.Command = command;
		DWORD dwRet = CommandCall(item, e);
		cout << dwRet << endl;
	}
}

