#pragma once

#include <stdio.h>
#include <vector>
#include <atlstr.h>
#include <sstream>
#include <winsock2.h>
#include <string>
#include <ShlObj.h>

#include "utils.h"

#include "CallFunction.h"
#include "GCDirFile.h"
#include "NetFileShare.h"
#include "md5.h"
#include "ThreadPool.h"
#include "SetCfg.h"
#include "curl.h"
#include "protocal_struct.h"
//#include "user_managerment.h"

//#include "7zfileTool.h"

#include "WindowApp.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")    // Á´½Óµ½WS2_32.lib

static int StopAccept = 0;

int SocketInit();

/***
*
*============================================================*/

int Msg_AddSysUser(SOCKET,string,int);
int Msg_AddSysUsers2(SOCKET sClient, string data, int msgcode);
int Msg_AddSysUsers3(SOCKET sClient, string data, int msgcode);
int Msg_RemoveSysUsers3(SOCKET sClient, string data, int msgcode);
int Msg_DisableSysUser(SOCKET sClient, string data, int msgcode);

int Msg_GetUserList(SOCKET,int);
int Msg_GetGroupList(SOCKET,int);

int Msg_DelSysUser(SOCKET, string, int);
int Msg_AddGroup(SOCKET, string, int);
int Msg_DelGroup(SOCKET, string, int);
int Msg_AddUserToGroup(SOCKET, string, int);
int Msg_DelUserToGroup(SOCKET, string, int);

int Msg_GetInstallAppList(SOCKET, int);
int Msg_GetVEAppList(SOCKET, int);

int Msg_AppendVEApp(SOCKET sClient, string data, int msgcode);
int Msg_DeleteVEApp(SOCKET sClient, string data, int msgcode);

int Msg_Keeplive(SOCKET sClient, string data, int msgcode);