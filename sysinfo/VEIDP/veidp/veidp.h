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

//ϵͳ��Ϣ����
void OnWMCreate(HWND hWnd);
void OnWMDevice(HWND hWnd, WPARAM wParam, LPARAM lParam);


void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void CALLBACK Tick_Pant(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void CALLBACK Tick_AIDA(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

//�����ļ�  ָ���ж�
void StartCreate();
void StartThread();
int ProcessMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt);
int ProcessMQThread(const char* msg);
void Tick();
void EndCreate();

//------------------------------------------------------MQ---------------------------------------------------------------------
void RocketMQLink(Json::Value& MQmessage, const char* MQtag);//����  ����  �ػ��õķ���MQ����
void StartSendMessage(CProducer *producer, Json::Value& MQmessage, string Tag, const string& ip);
static CProducer *producer = NULL;

void OnCommandCollAll();																	//�ռ�ȫ����Ϣ
void OnCommandHardware(bool bLaunch = false);							//�ռ�ȫ��Ӳ����Ϣ����������true������false
void OnEventSend();																			//�����¼�����������Ϣ

void instructScreenCapture(bool ifSortSent = false);							//��ͼ
void instructCreateMemory(bool ifSortSent = false);							//wmi�ڴ�
void instructPrinter(bool ifSortSent = false);										//wmi��ӡ��
void instructScreenInfo(bool ifSortSent = false);								//��Ļ��Ϣ
void SendDiskInfo(bool ifSortSent = false);
void SendCPUInfo(bool ifSortSent = false);
void SendMotherInfo(bool ifSortSent = false);
void SendKeyMouse(bool ifSortSent = false);
void SendUSB(bool bUpdate, bool ifSortSend = false);

static bool		aidaIf;		//	����������ʱ��ֻ��Ҫ����һ�� �κε���aida�ĺ���ִ�й�һ��
int  aide(bool force = false);								//����HIDA



#define LOG(str) IDPManager::Log.Log(str)
#define LOG_DAILY(str) IDPManager::Log.LogDaily(str)



