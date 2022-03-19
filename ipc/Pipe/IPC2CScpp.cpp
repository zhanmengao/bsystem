#include <windows.h>
#include <iostream>
using namespace std;

#define WRITEPIPENAME "\\\\.\\Pipe\\exeWrite"
#define READPIPENAME "\\\\.\\Pipe\\exeRead"

HANDLE hPipe;
HANDLE hWritePipe;
void ConsoleThread()
{
	while (1)
	{
		//打通管道
		if (WaitNamedPipe(TEXT(WRITEPIPENAME), NMPWAIT_WAIT_FOREVER) == FALSE)
		{
			cout << "等待命名管道实例失败！" << endl;
			Sleep(5000);
			continue;
		}
		HANDLE hPipe = CreateFile(TEXT(WRITEPIPENAME),
			GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipe == INVALID_HANDLE_VALUE)
		{
			cout << "连接命名管道失败！" << endl;
			CloseHandle(hPipe);
			Sleep(5000);
			continue;
		}
		//读输入
		char buffer[1024];
		DWORD WriteNum;
		cout << "已连接上服务器" << endl;
		while (1)
		{
			memset(buffer, 0, sizeof(buffer));
			cin >> buffer;
			if (WriteFile(hPipe, buffer, strlen(buffer), &WriteNum, NULL) == false)
			{
				cout << "数据写入管道失败！" << endl;
				cout << "关闭管道！" << endl;
				CloseHandle(hPipe);
				break;
			}
		}
	}
}

void main4()
{
	char buffer[1024];
	DWORD ReadNum;
	//创建了一个命名管道
	hPipe = CreateNamedPipe(TEXT(READPIPENAME),
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
		, PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, 0);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "创建命名管道失败！" << "   errorNo =" << GetLastError() << endl;
		CloseHandle(hPipe);
		return;
	}

	if (ConnectNamedPipe(hPipe, NULL) == FALSE)
	{
		cout << "与客户机连接失败！" << endl;
		CloseHandle(hPipe);
		return;
	}
	cout << "与客户机连接成功！" << endl;

	//创建线程 写数据
	HANDLE console = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsoleThread, NULL, 1, 0);
	ResumeThread(console);  //启动子线程

	while (1)
	{
		BOOL rRead = ReadFile(
			hPipe,
			buffer,
			1024,
			&ReadNum,
			0);
		if (rRead)
		{
			buffer[ReadNum] = '\0';
			printf("读出成功,c = %s\n", buffer);
		}
		else
		{
			cout << "读数据失败!关闭管道！" << endl;
			CloseHandle(hPipe);
		}
	}

	cout << "关闭管道！" << endl;
	CloseHandle(hPipe);
	CloseHandle(hWritePipe);
	system("pause");
}