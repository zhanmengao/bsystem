
#pragma once
#include "stdafx.h"
#include "CProducer.h"
#include "CMessage.h"
#include "CSendResult.h"
#include "CPushConsumer.h"
#include "CCommon.h"
#include "CMessageExt.h"
#include "json/json.h"

#include <Shlwapi.h>
#include <map>
#include <process.h>
#include <WbemCli.h>
#include <comutil.h>
#include <winver.h>

#include <comdef.h>  

#pragma comment(lib, "wbemuuid.lib")  
#pragma comment(lib, "version.lib")

#include <iostream>
#include <stdio.h>
#include <string>
#include <strsafe.h>
#include <Shlwapi.h>
#include <tchar.h>
#include "shellapi.h"

#include <iphlpapi.h>
#include <Winsock2.h>
#include <Setupapi.h>
#include <devguid.h>
#include <winioctl.h>
#include <Bthdef.h>
#include <vector>
#include <winreg.h>
#include <comdef.h>
#include <wbemidl.h>
#include <locale.h>
#include <tlhelp32.h>

#include <WbemCli.h>
#include <comutil.h>
#include <comdef.h>
#include"../../../../C++space/Convert/Convert/Convert.h"
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib,"Shlwapi.lib")
#define MAX_ALLSection		204800
#define MAX_SECTION			20     //段的最大长度

#pragma comment(lib,"wbemuuid.lib")

//#include "..//EasyWarInfo/WarInfo.h"
//#pragma comment(lib,"..//bin/EasyWarInfo.lib")

#define MINNUM 5

#include <strsafe.h>
#include <locale.h>

#define PROVIDER_NAME           L"System"
#define RESOURCE_DLL            L"c:\\windows\\system32\\wevtapi.dll"
#define MAX_TIMESTAMP_LEN       23 + 1   // mm/dd/yyyy hh:mm:ss.mmm
#define MAX_RECORD_BUFFER_SIZE  0x10000  // 64K

//MQ IF 
#define startMess		1
#define instructMess	2	
#define pantMess		3
#define endMess			4
#define deviceMess	5

#pragma comment( lib,"setupapi.lib")
#include <lm.h>
#pragma comment(lib,"netapi32.lib")


#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wbemuuid.lib")  
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"version.lib")
#pragma comment(lib,"Kernel32.lib")

using namespace std;

//监听文件  指令判断
enum COMMAND
{
	COMM_COLLALL = 1,
	COMM_SCREEN = 2,
	COMM_DISK = 3,
	COMM_MEMORY =  4,
	COMM_MONIT = 5,
	COMM_PRINTER = 6,
	COMM_KEY_MOUSE = 7,
	COMM_DOWNFILE = 8,
	COMM_HARDWARE = 9,
	COMM_CPU = 10,
	COMM_MAINBOARD = 11,
	COMM_USB = 12,
	COMM_EVENT = 13,
	COMM_START = 14,
	COMM_TICK = 15,
};

string GetApplicationDir();



string wbLoginName();

ULONG GetHostOrder(const string strIp);

Json::Value GetTCPportlist();
Json::Value GetUDPportlist();


string region();

string wbUptime();

string wbHostName();

string wbOsInfo(char* str);

string wbOsInfo11(char* str);


string wbOsCpuInfo(char* str);



struct soft_info
{
	string displayName;		//软件名称
	string displayVersion;	//版本
	string displaySize;		//大小
	string displayTime;		//安装时间
	string displayPost;		//发布者
};


Json::Value runProcess();


//aida=================================

string GetApplicationDir();
//SetCfg* get_client_config();
ULONG GetHostOrder(const string strIp);

typedef DWORD(WINAPI * PFN_GET_EXTENDED_TCP_TABLE)
(
	PVOID pTcpTable,
	PDWORD pdwSize,
	BOOL bOrder,
	ULONG ulAf,
	TCP_TABLE_CLASS TableClass,
	ULONG Reserved
	);
typedef DWORD(WINAPI * PFN_GET_EXTENDED_UDP_TABLE)
(
	PVOID pUdpTable,
	PDWORD pdwSize,
	BOOL bOrder,
	ULONG ulAf,
	UDP_TABLE_CLASS TableClass,
	ULONG Reserved
	);




/*=============== 软件打开关闭计数 ================


=============== 软件打开关闭计数 ================*/



#define MAX_ALLSECTIONS 300  //定义最大的段长度
#define MAX_SECTION 20     //段的最大长度
#define SystemTimeInformation 3



typedef struct {
	LARGE_INTEGER liKeBootTime;
	LARGE_INTEGER liKeSystemTime;
	LARGE_INTEGER liExpTimeZoneBias;
	ULONG uCurrentTimeZoneId;
	DWORD dwReserved;
} SYSTEM_TIME_INFORMATION;


typedef long(__stdcall *fnNtQuerySystemInformation)(
	IN     UINT SystemInformationClass,
	OUT PVOID SystemInformation,
	IN     ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

string GetBootTime();
string GetApplicationDir();


#define CONVERT(str) Convert::string_To_UTF8(str)

#define BUILD_MSG_HEAD(instructRoot)  net_info	netInfo = SysInfoUtils::GetNetInfo(); \
instructRoot["mac"] = netInfo.strMac;\
time_t t;\
t = time(NULL);\
time_t ii = time(&t);\
char str[32] = { 0 };\
sprintf(str, "%lld", ii);\
instructRoot["acquisitionTime"] = CONVERT(str)

