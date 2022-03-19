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
	//������Ϣ
	STARTUPINFO s_info;		//ָ������ʱ���̵Ĵ��ڹ���վ�����棬��׼����������ڵ����
	GetStartupInfo(&s_info);
	mAnon.Init(hrcmd, hwcmd);
	s_info.cb = sizeof(STARTUPINFO);
	s_info.lpReserved = NULL;
	s_info.wShowWindow = SW_HIDE;
	s_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	s_info.hStdError = hwcmd;
	s_info.hStdInput = hrcmd;
	s_info.hStdOutput = hwcmd;


	cout << "��ʼ�����" << endl;

	WCHAR cmdpath[1024] = L"\n";
	GetSystemDirectory(cmdpath, sizeof(cmdpath));
	wcsncat_s(cmdpath, L"\\cmd.exe", 9);	//���ַ������ӡ���

	wcout << "path:" << cmdpath << endl;

	//����cmd����
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