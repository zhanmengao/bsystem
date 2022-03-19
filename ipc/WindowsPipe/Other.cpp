#include <windows.h>
#include <iostream>
using namespace std;


char buffer[1024];
DWORD WriteNum;

void main0()
{
	while (1)
	{
		//打通管道
		if (WaitNamedPipe(TEXT("\\\\.\\Pipe\\namePipeTest"), NMPWAIT_WAIT_FOREVER) == FALSE)
		{
			cout << "等待命名管道实例失败！" << endl;
			Sleep(5000);
			continue;
		}
		HANDLE hPipe = CreateFile(TEXT("\\\\.\\Pipe\\namePipeTest"),
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