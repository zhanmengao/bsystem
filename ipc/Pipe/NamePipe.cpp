#include <windows.h>
#include <iostream>
using namespace std;

#define PIPENAME "\\\\.\\Pipe\\myPipe2"
#define CPIPENAME "\\\\.\\Pipe\\namePipeTest"
void main3()
{
	char buffer[1024];
	DWORD ReadNum;
	//������һ�������ܵ�
	HANDLE hPipe = CreateNamedPipe(TEXT(PIPENAME),
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
		, PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, 0);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "���������ܵ�ʧ�ܣ�" << "   errorNo =" << GetLastError() << endl;
		CloseHandle(hPipe);
		return;
	}

	if (ConnectNamedPipe(hPipe, NULL) == FALSE)
	{
		cout << "��ͻ�������ʧ�ܣ�" << endl;
		CloseHandle(hPipe);
		return;
	}
	cout << "��ͻ������ӳɹ���" << endl;

	while (1)
	{
		if (ReadFile(hPipe, buffer, 1024, &ReadNum, NULL) == FALSE)
		{
			cout << "��ȡ����ʧ�ܣ�" << endl;
			break;
		}

		buffer[ReadNum] = 0;
		cout << "��ȡ����:" << buffer << endl;
	}

	cout << "�رչܵ���" << endl;
	CloseHandle(hPipe);
	system("pause");
}