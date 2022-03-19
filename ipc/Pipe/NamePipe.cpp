#include <windows.h>
#include <iostream>
using namespace std;

#define PIPENAME "\\\\.\\Pipe\\myPipe2"
#define CPIPENAME "\\\\.\\Pipe\\namePipeTest"
void main3()
{
	char buffer[1024];
	DWORD ReadNum;
	//创建了一个命名管道
	HANDLE hPipe = CreateNamedPipe(TEXT(PIPENAME),
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

	while (1)
	{
		if (ReadFile(hPipe, buffer, 1024, &ReadNum, NULL) == FALSE)
		{
			cout << "读取数据失败！" << endl;
			break;
		}

		buffer[ReadNum] = 0;
		cout << "读取数据:" << buffer << endl;
	}

	cout << "关闭管道！" << endl;
	CloseHandle(hPipe);
	system("pause");
}