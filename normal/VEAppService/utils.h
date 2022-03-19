#pragma once
#include "stdafx.h"
#include "GStringClass.h"

struct net_info
{
	string strIp;
	string strMac;
};

class UtilsTools
{

public:
	UtilsTools();
	~UtilsTools();

public:
	GStringClass cstringClass;

public:
	string ltos(unsigned long l);
	int _GetNoOfProcessors();

	bool IsProcessRun(const char *pName);
	BOOL CallCommand(char *cmd, char *parameters);
	net_info GetNetInfo();
	string get_application_dir();
	tm get_current_time();
	BOOL CreateMultiDirectory(CString strPath);
private:
	ULONG GetHostOrder(const string strIp);
	VOID Byte2Hex(unsigned char bData, unsigned char hex[]);

};
