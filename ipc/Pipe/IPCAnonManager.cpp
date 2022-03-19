#include"IPCAnonManager.h"
#include <process.h>
static unsigned __stdcall ReadPipe(void* argv)
{
	PipeEventPair* pipe = (PipeEventPair*)(argv);
	ByteArray rData;
	while (1)
	{
		rData.clear();
		int iret = pipe->pipe->ReadData(rData);
		if (iret == -1)
		{
			break;
		}
		if (!rData.empty())
		{
			pipe->pe(rData, pipe->argv);
		}
		Sleep(200);
	}
	return 0;
}

IPCAnon::~IPCAnon()
{
	Destroy();
}
bool IPCAnon::Init(HANDLE &nRead, HANDLE& nWrite, PipeEvent e, void* argv)
{
	nRead = NULL;
	nWrite = NULL;
	SECURITY_ATTRIBUTES sa;		//Windows安全设置:此结构为各种函数创建的对象提供安全设置
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;
	bool ret = true;
	//mRead nWrite
	if (!CreatePipe(&mRead, &nWrite, &sa, 0))
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	if (!CreatePipe(&nRead, &mWrite, &sa, 0))
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	if (!ret)
	{
		Destroy();
	}
	else if(e)
	{
		hThreadData = new PipeEventPair();
		hThreadData->pe = e;
		hThreadData->pipe = this;
		hThreadData->argv = argv;
		hThread = (HANDLE)(_beginthreadex(NULL, NULL, ReadPipe, hThreadData,NULL,NULL));
	}
	return ret;
}

ByteArray IPCAnon::Destroy()
{
	ByteArray data;
	//读完再关闭
	ReadNonBlock(data);
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
	if (hThread != NULL)
	{
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		hThread = NULL;
	}
	if (hThreadData)
	{
		delete hThreadData;
		hThreadData = NULL;
	}
	return data;
}