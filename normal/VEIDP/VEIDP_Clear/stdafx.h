// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifdef _WIN32
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>
#endif
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#ifdef _WIN32
#include <tchar.h>
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Error/WErrorUtils/ErrorInfo.h"
#endif
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
	COMM_INVALID = 0,					//��Ч��Ϣ
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
	MQ_LOCAL,						//������
	MQ_WAN,							//������
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
