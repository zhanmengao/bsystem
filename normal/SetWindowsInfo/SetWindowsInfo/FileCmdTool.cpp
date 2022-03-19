#include "stdafx.h"
#include "FileCmdTool.h"
#include <io.h>

FileCmdTool::FileCmdTool()
{
}


FileCmdTool::~FileCmdTool()
{
}

int FileCmdTool::SetSysFile2AdminACL( wchar_t* filename  )
{

	
	wchar_t cmds[MAX_PATH] = { 0 };
	wchar_t *tname = filename;

	//lstrcpy( tname ,filename  );

	/**
	takeown /f c:\windows\HelpPane.exe && icacls c:\windows\HelpPane.exe  /grant administrators:F
	**/
	wsprintf(cmds, L"takeown /f \"%ls\" && icacls \"%ls\" /grant administrators:F", tname, tname );
	
	int rCode = CallCmd2(cmds);

	switch (rCode)
	{

	case 0:
		break;   //成功 

	case 5:      //拒绝访问
		break;

	case 1332:   //帐户名与安全标识间无任何映射完成。
		break;

	default:
		break;
	}

	return rCode;


}

int FileCmdTool::CallCmd2(wchar_t* cmdline)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	BOOL working = ::CreateProcess(
		NULL,
		cmdline,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&si,
		&pi);

	if (working == 0)
	{
		DWORD error = GetLastError();
		return error;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	unsigned long Result;
	GetExitCodeProcess(pi.hProcess, &Result);

	return Result;
}



int FileCmdTool::FileRename( wchar_t* source, wchar_t* newname)
{
		
	int error = -1;

	if (!_waccess(source, 0))
	{
		if (!_wrename(source, newname))
		{
			error = 0;
		}
		else
		{
			error = -2;
		}
	}
	else 
	{
		error = -1;
	}

	return error;
}