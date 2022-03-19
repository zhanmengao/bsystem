#include"IPCAnonManager.h"
#include"IPCNameManager.h"
using namespace std;
#include<iostream>
#include<thread>
#include<tchar.h>
const stdstr serverReadPath(_T("\\\\.\\Pipe\\myread"));
const stdstr serverWritePath(_T("\\\\.\\Pipe\\mywrite"));
IPCAnon mAnon;
IPCName mName;
void ReadAnonMain()
{
	ByteArray buffer;
	while (1)
	{
		if (mAnon.ReadData(buffer,false))
		{
			if (buffer.size() > 0)
			{
				cout << "Read Anon:" << string(buffer.data(),buffer.size()) << endl;
			}
			continue;
		}
		Sleep(1000);
	}
}
void mainAnon()
{
	HANDLE hrcmd = NULL;
	HANDLE hwcmd = NULL;
	PROCESS_INFORMATION pro_info;
	//启动信息
	STARTUPINFO s_info;		//指定创建时进程的窗口工作站，桌面，标准句柄和主窗口的外观
	GetStartupInfo(&s_info);
	mAnon.Init(hrcmd, hwcmd);
	s_info.cb = sizeof(STARTUPINFO);
	s_info.lpReserved = NULL;
	s_info.wShowWindow = SW_HIDE;
	s_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	s_info.hStdError = hwcmd;
	s_info.hStdInput = hrcmd;
	s_info.hStdOutput = hwcmd;


	cout << "初始化完成" << endl;

	WCHAR cmdpath[1024] = L"\n";
	GetSystemDirectory(cmdpath, sizeof(cmdpath));
	wcsncat_s(cmdpath, L"\\cmd.exe", 9);	//宽字符串连接。。

	wcout << "path:" << cmdpath << endl;

	//创建cmd进程
	if (!CreateProcess(cmdpath, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &s_info, &pro_info))
	{
		mAnon.Destroy();
	}
	std::thread rThread(ReadAnonMain);
	char buf[1024];
	while (1)
	{
		cin.getline(buf, 1024);
		size_t size = cin.gcount();
		string str(buf);
		str += "\r\n";
		if (!mAnon.WriteData(str.data(), str.size()))
		{
			cout << "Write error!" << endl;
		}
	}
}
void ReadNameMain()
{
	ByteArray buffer;
	while (1)
	{
		if (mName.ReadData(buffer))
		{
			if (buffer.size() > 0)
			{
				cout << "Read Name:" << string(buffer.data(), buffer.size()) << endl;
			}
		}
		Sleep(1000);
	}
}
void mainName()
{
	mName.InitServer(serverReadPath, serverWritePath);
	std::thread rThread(ReadNameMain);
	char buf[1024];
	while (1)
	{
		cin.getline(buf, 1024);
		if (!mName.WriteData(buf,cin.gcount()))
		{
			cout << "Write error!" << endl;
		}
	}
}

void main()
{
	mainAnon();
	//mainName();
}