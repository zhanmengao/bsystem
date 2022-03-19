// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件
#define SEND_IP_LABEL 10001
#define SEND_IP_INPUT 10002
#define SEND_PORT_LABEL 10003
#define SEND_PORT_INPUT 10004
#define SEND_TOPIC_LABEL 10005
#define SEND_TOPIC_INPUT 10006

#define RECV_IP_LABEL 10007
#define RECV_IP_INPUT 10008
#define RECV_PORT_LABEL 10009
#define RECV_PORT_INPUT 10010
#define RECV_TOPIC_LABEL 10011
#define RECV_TOPIC_INPUT 10012

#define BTN_ENTER 10013
#define BTN_CANCEL 10014

#define INPUT_TIMER 10015
#define INPUT_ORG 10016