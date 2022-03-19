#include"IPCNameManager.h"
#include <iostream>
bool IPCName::InitServer(const stdstr& readPipeName, const stdstr& writePipeName)
{
	bool ret = true;
	do
	{
		//������һ�������ܵ�
		mRead = CreateNamedPipe(readPipeName.c_str(),
			PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
			, PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, 0);
		if (mRead == INVALID_HANDLE_VALUE)
		{
			cout << "���������ܵ�ʧ�ܣ�" << "   errorNo =" << GetLastError() << endl;
			ret = false;
			break;
		}

		if (ConnectNamedPipe(mRead, NULL) == FALSE)
		{
			cout << "��ͻ�������ʧ�ܣ�" << endl;
			ret = false;
			break;
		}

		while (1)
		{
			//��ͨ�ܵ�
			if (WaitNamedPipe(writePipeName.c_str(), NMPWAIT_WAIT_FOREVER) == FALSE)
			{
				cout << "�ȴ������ܵ�ʵ��ʧ�ܣ�" << endl;
				Sleep(1000);
				continue;
			}
			mWrite = CreateFile(writePipeName.c_str(),
				GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (mWrite == INVALID_HANDLE_VALUE)
			{
				cout << "���������ܵ�ʧ�ܣ�" << endl;;
				Sleep(1000);
				continue;
			}
			else
			{
				break;
			}
		}
	} while (false);
	if (!ret)
	{
		Close();
	}
	if (ret)
	{
		cout << "�ܵ������ɹ���" << endl;
	}
	else
	{
		cout << "�ܵ�����ʧ�ܣ�" << endl;
	}
	return ret;
}
bool IPCName::InitClient(const stdstr& readPipeName, const stdstr& writePipeName)
{
	bool ret = true;
	do
	{
		while (1)
		{
			//��ͨ�ܵ�
			if (WaitNamedPipe(writePipeName.c_str(), NMPWAIT_WAIT_FOREVER) == FALSE)
			{
				cout << "�ȴ������ܵ�ʵ��ʧ�ܣ�" << endl;
				Sleep(1000);
				continue;
			}
			mWrite = CreateFile(writePipeName.c_str(),
				GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (mWrite == INVALID_HANDLE_VALUE)
			{
				cout << "���������ܵ�ʧ�ܣ�" << endl;;
				Sleep(1000);
				continue;
			}
			else
			{
				break;
			}
		}
		//������һ�������ܵ�
		mRead = CreateNamedPipe(readPipeName.c_str(),
			PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
			, PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, 0);
		if (mRead == INVALID_HANDLE_VALUE)
		{
			cout << "���������ܵ�ʧ�ܣ�" << "   errorNo =" << GetLastError() << endl;
			ret = false;
			break;
		}

		if (ConnectNamedPipe(mRead, NULL) == FALSE)
		{
			cout << "��ͻ�������ʧ�ܣ�" << endl;
			ret = false;
			break;
		}

	} while (false);
	if (!ret)
	{
		if (mRead != NULL)
		{
			CloseHandle(mRead);
			mRead = NULL;
		}
		if (mWrite != NULL)
		{
			CloseHandle(mWrite);
			mWrite = NULL;
		}
	}
	return ret;
}
void IPCName::Close()
{
	if (mRead != NULL)
	{
		CloseHandle(mRead);
		mRead = NULL;
	}
	if (mWrite != NULL)
	{
		CloseHandle(mWrite);
		mWrite = NULL;
	}
}