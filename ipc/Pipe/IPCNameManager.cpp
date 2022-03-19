#include"IPCNameManager.h"
#include <iostream>
bool IPCName::InitServer(const stdstr& readPipeName, const stdstr& writePipeName)
{
	bool ret = true;
	do
	{
		//创建了一个命名管道
		mRead = CreateNamedPipe(readPipeName.c_str(),
			PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
			, PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, 0);
		if (mRead == INVALID_HANDLE_VALUE)
		{
			cout << "创建命名管道失败！" << "   errorNo =" << GetLastError() << endl;
			ret = false;
			break;
		}

		if (ConnectNamedPipe(mRead, NULL) == FALSE)
		{
			cout << "与客户机连接失败！" << endl;
			ret = false;
			break;
		}

		while (1)
		{
			//打通管道
			if (WaitNamedPipe(writePipeName.c_str(), NMPWAIT_WAIT_FOREVER) == FALSE)
			{
				cout << "等待命名管道实例失败！" << endl;
				Sleep(1000);
				continue;
			}
			mWrite = CreateFile(writePipeName.c_str(),
				GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (mWrite == INVALID_HANDLE_VALUE)
			{
				cout << "连接命名管道失败！" << endl;;
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
		cout << "管道创建成功！" << endl;
	}
	else
	{
		cout << "管道创建失败！" << endl;
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
			//打通管道
			if (WaitNamedPipe(writePipeName.c_str(), NMPWAIT_WAIT_FOREVER) == FALSE)
			{
				cout << "等待命名管道实例失败！" << endl;
				Sleep(1000);
				continue;
			}
			mWrite = CreateFile(writePipeName.c_str(),
				GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (mWrite == INVALID_HANDLE_VALUE)
			{
				cout << "连接命名管道失败！" << endl;;
				Sleep(1000);
				continue;
			}
			else
			{
				break;
			}
		}
		//创建了一个命名管道
		mRead = CreateNamedPipe(readPipeName.c_str(),
			PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
			, PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, 0);
		if (mRead == INVALID_HANDLE_VALUE)
		{
			cout << "创建命名管道失败！" << "   errorNo =" << GetLastError() << endl;
			ret = false;
			break;
		}

		if (ConnectNamedPipe(mRead, NULL) == FALSE)
		{
			cout << "与客户机连接失败！" << endl;
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