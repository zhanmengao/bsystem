#pragma comment(lib,"user32")
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <string>
int maino3(int argc, char*argv[])
{
	//std::wstring cmdLine(L"C:/Windows/System32/notepad.exe F:/222.txt");
	std::wstring cmdLine(L"C:/Windows/System32/cmd.exe whoami");
	//LPWSTR cmdLine = ;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;//指定wShowWindow成员有效
	si.wShowWindow = TRUE;//此成员设为TRUE的话则显示新建进程的主窗口
	//system("notepad.exe F://222.txt");
	//system("cmd.exe /c whoami");
	BOOL bRet = CreateProcess(
		NULL,        //不在此指定可执行文件的文件名
		(LPWSTR)cmdLine.c_str(),//命令行参数
		NULL,//默认进程安全性
		NULL,//默认进程安全性
		TRUE,//指定当前进程内句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	if (bRet)
	{
		//不使用的句柄最好关掉
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		printf("新进程的ID号：%d\n", pi.dwProcessId);
		printf("新进程的主线程ID号：%d\n", pi.dwThreadId);
	}
	else
	{
		auto ret = GetLastError();
		printf("创建失败！,errno = %u\n",ret);
	}
	return 0;
}

int main1() 
{
	SECURITY_ATTRIBUTES sa = { 0 };
	STARTUPINFO         si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	HANDLE              hPipeOutputRead = NULL;
	HANDLE              hPipeOutputWrite = NULL;
	HANDLE              hPipeInputRead = NULL;
	HANDLE              hPipeInputWrite = NULL;
	BOOL                bTest = 0;
	DWORD               dwNumberOfBytesRead = 0;
	DWORD               dwNumberOfBytesWrite = 0;
	CHAR                szMsg[100];
	CHAR                szBuffer[256];

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create pipe for standard output redirection.
	CreatePipe(&hPipeOutputRead,  // read handle
		&hPipeOutputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Create pipe for standard input redirection.
	CreatePipe(&hPipeInputRead,  // read handle
		&hPipeInputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Make child process use hPipeOutputWrite as standard out,
	// and make sure it does not show on screen.
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = hPipeInputRead;
	si.hStdOutput = hPipeOutputWrite;
	si.hStdError = hPipeOutputWrite;

	CreateProcess(
		NULL, L"cmd.exe",
		NULL, NULL,
		TRUE, 0,
		NULL, NULL,
		&si, &pi);

	// Now that handles have been inherited, close it to be safe.
	// You don't want to read or write to them accidentally.
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	// Now test to capture DOS application output by reading
	// hPipeOutputRead.  Could also write to DOS application
	// standard input by writing to hPipeInputWrite.
	sprintf(szMsg, "ver\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}

	sprintf(szMsg, "chcp\nexit\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
	}

	// Wait for CONSPAWN to finish.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close all remaining handles
	CloseHandle(pi.hProcess);
	CloseHandle(hPipeOutputRead);
	CloseHandle(hPipeInputWrite);

	return 0;
}

int main2() 
{
	SECURITY_ATTRIBUTES sa = { 0 };
	STARTUPINFO         si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	HANDLE              hPipeOutputRead = NULL;
	HANDLE              hPipeOutputWrite = NULL;
	HANDLE              hPipeInputRead = NULL;
	HANDLE              hPipeInputWrite = NULL;
	BOOL                bTest = 0;
	DWORD               dwNumberOfBytesRead = 0;
	DWORD               dwNumberOfBytesWrite = 0;
	CHAR                szMsg[100];
	CHAR                szBuffer[256];

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create pipe for standard output redirection.
	CreatePipe(&hPipeOutputRead,  // read handle
		&hPipeOutputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Create pipe for standard input redirection.
	CreatePipe(&hPipeInputRead,  // read handle
		&hPipeInputWrite, // write handle
		&sa,      // security attributes
		0      // number of bytes reserved for pipe - 0 default
		);

	// Make child process use hPipeOutputWrite as standard out,
	// and make sure it does not show on screen.
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = hPipeInputRead;
	si.hStdOutput = hPipeOutputWrite;
	si.hStdError = hPipeOutputWrite;

	CreateProcess(
		NULL, L"cmd.exe",
		NULL, NULL,
		TRUE, 0,
		NULL, NULL,
		&si, &pi);

	// Now that handles have been inherited, close it to be safe.
	// You don't want to read or write to them accidentally.
	CloseHandle(hPipeOutputWrite);
	CloseHandle(hPipeInputRead);

	// Now test to capture DOS application output by reading
	// hPipeOutputRead.  Could also write to DOS application
	// standard input by writing to hPipeInputWrite.
	sprintf(szMsg, "cd /d c:\\user\\lsw\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}

	sprintf(szMsg, "adb forward tcp:31415 tcp:31415\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}

	sprintf(szMsg, "drozer console connect\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}

	sprintf(szMsg, "help\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}

	sprintf(szMsg, "quit\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}

	sprintf(szMsg, "exit\n");
	WriteFile(
		hPipeInputWrite,      // handle of the write end of our pipe
		&szMsg,               // address of buffer that send data
		strlen(szMsg),        // number of bytes to write
		&dwNumberOfBytesWrite,// address of number of bytes read
		NULL                  // non-overlapped.
		);

	while (TRUE)
	{
		bTest = ReadFile(
			hPipeOutputRead,      // handle of the read end of our pipe
			&szBuffer,            // address of buffer that receives data
			256,                  // number of bytes to read
			&dwNumberOfBytesRead, // address of number of bytes read
			NULL                  // non-overlapped.
			);

		if (!bTest){
			sprintf(szMsg, "Error #%d reading pipe.", GetLastError());
			printf("%s", szMsg);
			break;
		}

		// do something with data.
		szBuffer[dwNumberOfBytesRead] = 0;  // null terminate
		printf("%s", szBuffer);
		if ('>' == szBuffer[dwNumberOfBytesRead - 1]) break;
	}
	// Wait for CONSPAWN to finish.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close all remaining handles
	CloseHandle(pi.hProcess);
	CloseHandle(hPipeOutputRead);
	CloseHandle(hPipeInputWrite);

	return 0;
}
//C:\test>test
//Microsoft Windows [版本 5.2.3790]
//(C) 版权所有 1985-2003 Microsoft Corp.
//
//C:\test>ver
//
//Microsoft Windows [版本 5.2.3790]
//
//C:\test>chcp
//活动的代码页: 936
//
//C:\test>exit
//Error #109 reading pipe.
//C:\test>


