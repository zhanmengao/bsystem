#include"../../PipeServer/Pipe/IPCNameManager.h"
#include<iostream>
#include<thread>
using namespace std;
IPCName mName;
const string serverReadPath("\\\\.\\Pipe\\myread");
const string serverWritePath("\\\\.\\Pipe\\mywrite");
void ReadNameMain()
{
	string buffer;
	while (1)
	{
		buffer = "";
		if (mName.ReadData(buffer))
		{
			cout << "Read Name:" << buffer << endl;
			continue;
		}
		Sleep(1000);
	}
}
void mainName()
{
	mName.InitClient(serverWritePath, serverReadPath);
	std::thread rThread(ReadNameMain);
	char buf[1024];
	while (1)
	{
		cin.getline(buf, 1024);
		if (!mName.WriteData(buf))
		{
			cout << "Write error!" << endl;
		}
	}
}

void main()
{
	mainName();
}