// win_pipe.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <tchar.h>

const auto SEND_BUFF_SIZE = 1024;

using namespace std;

inline void winerr(void);		//用于打印windows错误代码

int main2()
{
	HANDLE hrcmd = NULL;
	HANDLE hwcmd = NULL;
	HANDLE hrexe = NULL;
	HANDLE hwexe = NULL;
	HANDLE hpro = NULL;

	SECURITY_ATTRIBUTES sa;		//Windows安全设置:此结构为各种函数创建的对象提供安全设置
	PROCESS_INFORMATION pro_info;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;

	if (!CreatePipe(&hrexe, &hwcmd, &sa, 0))
	{
		winerr();
		return 1;
	}
	else
	{
		cout << "creat pipe1 noerror" << endl;
	}
	if (!CreatePipe(&hrcmd, &hwexe, &sa, 0))
	{
		winerr();
		return 1;
	}
	else
	{
		cout << "creat pipe2 noerror" << endl;
	}

	//启动信息

	STARTUPINFO s_info;		//指定创建时进程的窗口工作站，桌面，标准句柄和主窗口的外观
	GetStartupInfo(&s_info);
	s_info.cb = sizeof(STARTUPINFO);
	s_info.lpReserved = NULL;
	s_info.wShowWindow = SW_HIDE;
	s_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	s_info.hStdError = hwcmd;
	s_info.hStdInput = hrcmd;
	s_info.hStdOutput = hwcmd;

	cout << "初始化完成" << endl;

	WCHAR cmdpath[1024] = _T("\n");
	GetSystemDirectory(cmdpath, sizeof(cmdpath));
	wcsncat_s(cmdpath, _T("\\cmd.exe"), 9);	//宽字符串连接。。

	wcout << "path:" << cmdpath << endl;

	//创建cmd进程
	if (!CreateProcess(cmdpath, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &s_info, &pro_info))
	{
		winerr();
		CloseHandle(hwexe);
		CloseHandle(hrexe);
		return 1;
	}

	cout << "开始读写管道...." << endl;
	cout << "注意：为了完整读取例如 ping等需要等待时间的命令的返回数据在输入数据后会有5秒延迟" << endl
		<< "输入END即退出该程序" << endl;

	//---注意!!!  CMD是ASCII编码用WCHAR(UNICODE编码)会乱码
	char rbuf[2048];
	unsigned long rn = 0, wn = 0;
	//----------------------------读写管道----------------------------
	do
	{
		do
		{
			if (!PeekNamedPipe(hrexe, NULL, NULL, NULL, &rn, 0))	//管道是否有数据可读
			{
				cout << "nodata" << endl;
				winerr();
				break;
			}
			if ((rn > 0) && (rn < 2048))
			{
				memset(rbuf, 0, 2048);
				if (!ReadFile(hrexe, rbuf, rn, NULL, NULL))
				{
					cout << "读管道错误" << endl;
					winerr();
					break;
				}
				else
				{
					cout << rbuf;
				}
			}
			Sleep(1000);
		} while (rn != 0);
		//---------写管道---------
		string wbuf;
		getline(cin, wbuf);
		if (wbuf == "END")
		{
			break;
		}
		wbuf += "\r\n";
		if (!WriteFile(hwexe, wbuf.c_str(), strlen(wbuf.c_str()), &wn, NULL))
		{
			cout << "写管道错误" << endl;
			winerr();
			break;
		}
		else
		{
			cout << wbuf;
		}
		Sleep(500);
	} while (true);

	TerminateProcess(pro_info.hProcess, 0);		//用于关闭cmd进程
	return 0;

}

inline void winerr(void)
{
	cout << "win error:" << GetLastError() << endl;
}