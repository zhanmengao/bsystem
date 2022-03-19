// win_pipe.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <tchar.h>

const auto SEND_BUFF_SIZE = 1024;

using namespace std;

inline void winerr(void);		//���ڴ�ӡwindows�������

int main2()
{
	HANDLE hrcmd = NULL;
	HANDLE hwcmd = NULL;
	HANDLE hrexe = NULL;
	HANDLE hwexe = NULL;
	HANDLE hpro = NULL;

	SECURITY_ATTRIBUTES sa;		//Windows��ȫ����:�˽ṹΪ���ֺ��������Ķ����ṩ��ȫ����
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

	//������Ϣ

	STARTUPINFO s_info;		//ָ������ʱ���̵Ĵ��ڹ���վ�����棬��׼����������ڵ����
	GetStartupInfo(&s_info);
	s_info.cb = sizeof(STARTUPINFO);
	s_info.lpReserved = NULL;
	s_info.wShowWindow = SW_HIDE;
	s_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	s_info.hStdError = hwcmd;
	s_info.hStdInput = hrcmd;
	s_info.hStdOutput = hwcmd;

	cout << "��ʼ�����" << endl;

	WCHAR cmdpath[1024] = _T("\n");
	GetSystemDirectory(cmdpath, sizeof(cmdpath));
	wcsncat_s(cmdpath, _T("\\cmd.exe"), 9);	//���ַ������ӡ���

	wcout << "path:" << cmdpath << endl;

	//����cmd����
	if (!CreateProcess(cmdpath, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &s_info, &pro_info))
	{
		winerr();
		CloseHandle(hwexe);
		CloseHandle(hrexe);
		return 1;
	}

	cout << "��ʼ��д�ܵ�...." << endl;
	cout << "ע�⣺Ϊ��������ȡ���� ping����Ҫ�ȴ�ʱ�������ķ����������������ݺ����5���ӳ�" << endl
		<< "����END���˳��ó���" << endl;

	//---ע��!!!  CMD��ASCII������WCHAR(UNICODE����)������
	char rbuf[2048];
	unsigned long rn = 0, wn = 0;
	//----------------------------��д�ܵ�----------------------------
	do
	{
		do
		{
			if (!PeekNamedPipe(hrexe, NULL, NULL, NULL, &rn, 0))	//�ܵ��Ƿ������ݿɶ�
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
					cout << "���ܵ�����" << endl;
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
		//---------д�ܵ�---------
		string wbuf;
		getline(cin, wbuf);
		if (wbuf == "END")
		{
			break;
		}
		wbuf += "\r\n";
		if (!WriteFile(hwexe, wbuf.c_str(), strlen(wbuf.c_str()), &wn, NULL))
		{
			cout << "д�ܵ�����" << endl;
			winerr();
			break;
		}
		else
		{
			cout << wbuf;
		}
		Sleep(500);
	} while (true);

	TerminateProcess(pro_info.hProcess, 0);		//���ڹر�cmd����
	return 0;

}

inline void winerr(void)
{
	cout << "win error:" << GetLastError() << endl;
}