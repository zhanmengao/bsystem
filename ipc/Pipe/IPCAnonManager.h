#pragma once
#ifdef _WIN32
#include<windows.h>
#else
#include<unistd.h>
#endif
#include<string>
#include"AbstractIPC.h"
using namespace std;

typedef void(*PipeEvent)(const ByteArray& rData,void* argv);
class IPCAnon;
struct PipeEventPair
{
	PipeEvent pe;
	IPCAnon* pipe;
	void* argv;
	bool bOpen = true;
};
//ÄäÃû¹ÜµÀ
class IPCAnon:public AbstractIPC
{
public:
	IPCAnon()
	{

	}
	~IPCAnon();
	bool Init(HANDLE &nRead, HANDLE& nWrite, PipeEvent = nullptr,void* argv = nullptr);
	ByteArray Destroy();
private:
	char rbuf[2048];
	unsigned long rn = 0, wn = 0;

	HANDLE hThread = NULL;
	PipeEventPair* hThreadData = NULL;
};
