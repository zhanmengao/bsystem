#pragma once
#include "resource.h"
#include "json/json.h"
#include "CProducer.h"
#include "CMessage.h"
#include "CSendResult.h"
#include "CPushConsumer.h"
#include "CCommon.h"
#include "CMessageExt.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"GlobalData.h"

#pragma comment(lib,"jsoncpp_static.lib")
#pragma comment(lib,"rocketmq-client-cpp.lib")
#include<string>
using namespace std;

#ifdef _DEBUG
#define __DEBUG 1
#endif

#define __TEST_VM 0

#define __TASK_ 0

//系统消息处理
void OnWMCreate(HWND hWnd);
void OnWMDevice(HWND hWnd, WPARAM wParam, LPARAM lParam);


void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void CALLBACK Tick_Pant(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void CALLBACK Tick_AIDA(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

//监听文件  指令判断
void StartCreate();
void StartThread();
int ProcessMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt);
int ProcessMQThread(const char* msg);
void Tick();
void EndCreate();

//------------------------------------------------------MQ---------------------------------------------------------------------
void RocketMQLink(Json::Value& MQmessage, const char* MQtag);//开机  心跳  关机用的发送MQ数据
void StartSendMessage(CProducer *producer, Json::Value& MQmessage, string Tag, const string& ip);
static CProducer *producer = NULL;

void OnCommandCollAll();																	//收集全部信息
void OnCommandHardware(bool bLaunch = false);							//收集全部硬件信息，主动发传true，被动false
void OnEventSend();																			//特殊事件发生，发消息

void instructScreenCapture(bool ifSortSent = false);							//截图
void instructCreateMemory(bool ifSortSent = false);							//wmi内存
void instructPrinter(bool ifSortSent = false);										//wmi打印机
void instructScreenInfo(bool ifSortSent = false);								//屏幕信息
void SendDiskInfo(bool ifSortSent = false);
void SendCPUInfo(bool ifSortSent = false);
void SendMotherInfo(bool ifSortSent = false);
void SendKeyMouse(bool ifSortSent = false);
void SendUSB(bool bUpdate, bool ifSortSend = false);

static bool		aidaIf;		//	程序启动的时候只需要启动一次 任何调用aida的函数执行过一次
int  aide(bool force = false);								//唤起HIDA



#define LOG(str) IDPManager::Log.Log(str)
#define LOG_DAILY(str) IDPManager::Log.LogDaily(str)



