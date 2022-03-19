// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef _WIN32
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#endif
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#ifdef _WIN32
#include <tchar.h>
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Error/WErrorUtils/ErrorInfo.h"
#endif
// TODO:  在此处引用程序需要的其他头文件
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/LogThread.h"

#ifdef _WIN32
#define CONVERT(str)  Convert::string_To_UTF8(str)
#else
#define CONVERT(str)  str
#endif

#define LOG(str) LogUtils::Log(str)
#define LOG_DAILY(str) LogUtils::LogDaily(str)


#define BUILD_MSG_HEAD(instructRoot)  \
instructRoot["mac"] = netInfo.strMac;\
time_t t;\
t = time(NULL);\
time_t ii = time(&t);\
char str[32] = { 0 };\
sprintf(str, "%lld", ii);\
instructRoot["acquisitionTime"] = CONVERT(str)
enum COMMAND
{
	COMM_INVALID = 0,					//无效消息
	COMM_COLLALL = 1,
	COMM_SCREEN = 2,
	COMM_DISK = 3,
	COMM_MEMORY = 4,
	COMM_MONIT = 5,
	COMM_PRINTER = 6,
	COMM_KEY_MOUSE = 7,
	COMM_VERSION = 8,
	COMM_HARDWARE = 9,
	COMM_CPU = 10,
	COMM_MAINBOARD = 11,
	COMM_USB = 12,
	COMM_EVENT = 13,
	COMM_START = 14,
	COMM_TICK = 15,
	COMM_CACHE = 16,
	COMM_EXIT = 17,
	COMM_MSGBOX = 18,
	COMM_ORGCODE = 19,
	COMM_CLOSE = 20,
	COMMAND_MAX,
};

enum MQ_TYPE
{
	MQ_LOCAL,						//局域网
	MQ_WAN,							//广域网
};

enum TAG_DEF
{
	TAG_START = 0,
	TAG_PANT,
	TAG_INSTRUCT,
	TAG_END,
	TAG_EVENT,
	TAG_CACHE,
	TAG_MAX
};
