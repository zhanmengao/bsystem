#pragma once
#include<windows.h>
#include<string>
#include"AbstractIPC.h"
#ifndef _LOCAL
#define _LOCAL 1
#endif
#if _LOCAL
#include"../../../C++space/StringTools/StringTools/StringHead.h"
#else
#include"StringHead.h"
#endif
using namespace std;

//ÄäÃû¹ÜµÀ
class IPCName:public AbstractIPC
{
public:
	IPCName()
	{
	}
	~IPCName()
	{
		Close();
	}
	bool InitServer(const stdstr& readPipeName,const stdstr& writePipeName);
	bool InitClient(const stdstr& readPipeName, const stdstr& writePipeName);
	void Close();
private:
};