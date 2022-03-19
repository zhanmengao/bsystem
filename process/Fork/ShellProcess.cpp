#include"ShellProcess.h"

ShellProcess::ShellProcess()
{
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;//_T("runs");
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
}
ShellProcess::~ShellProcess()
{
	if (ShExecInfo.hProcess)
	{
		CloseHandle(ShExecInfo.hProcess);
		ShExecInfo.hProcess = NULL;
	}
}
void ShellProcess::SetPath(const stdstr& processPath, const stdstr& command)
{
	ShExecInfo.lpFile = processPath.c_str();
	ShExecInfo.lpParameters = command.c_str();
}
BOOL ShellProcess::Run()
{
	BOOL bRet = ShellExecuteEx(&ShExecInfo);

	return bRet;
}
DWORD ShellProcess::Wait(DWORD dwMilliseconds)
{
	return WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds);
}
DWORD ShellProcess::ExitCode()
{
	DWORD dwExit = 0;
	GetExitCodeProcess(ShExecInfo.hProcess, &dwExit);
	return dwExit;				//STILL_ACTIVE = 0x103
}
