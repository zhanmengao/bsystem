#include <windows.h>
#include <iostream>
using namespace std;


char buffer[1024];
DWORD WriteNum;

void main0()
{
	while (1)
	{
		//��ͨ�ܵ�
		if (WaitNamedPipe(TEXT("\\\\.\\Pipe\\namePipeTest"), NMPWAIT_WAIT_FOREVER) == FALSE)
		{
			cout << "�ȴ������ܵ�ʵ��ʧ�ܣ�" << endl;
			Sleep(5000);
			continue;
		}
		HANDLE hPipe = CreateFile(TEXT("\\\\.\\Pipe\\namePipeTest"),
			GENERIC_READ | GENERIC_WRITE, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hPipe == INVALID_HANDLE_VALUE)
		{
			cout << "���������ܵ�ʧ�ܣ�" << endl;
			CloseHandle(hPipe);
			Sleep(5000);
			continue;
		}
		//������
		cout << "�������Ϸ�����" << endl;
		while (1)
		{
			memset(buffer, 0, sizeof(buffer));
			cin >> buffer;
			if (WriteFile(hPipe, buffer, strlen(buffer), &WriteNum, NULL) == false)
			{
				cout << "����д��ܵ�ʧ�ܣ�" << endl;
				cout << "�رչܵ���" << endl;
				CloseHandle(hPipe);
				break;
			}
		}
	}
}