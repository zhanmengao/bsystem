#pragma once
#include"../../../../Network/MQ/RocketMQ/RocketMQ/RockMQProduUtils.h"
#include"../../../../Network/MQ/RocketMQ/PushConsumer/RockMQConsuUtils.h"
#include"../../../USB/WUSBDevice/USBUtils.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessMonitWMI.h"
#include<thread>
#include<json/json.h>
#include<mutex>
#include<set>
#include"infoCollect.h"
class UserManager
{
public:
	void Start();
	void  TimerCall();
	void  Destroy();
	std::map<stdstr, DWORD> MonitProcessEnd();
	void  SendALL();
	int ProcessMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt);						//»Øµ÷
	const std::map<stdstr, USBDevice>& GetUsbMap() const;
	std::map<stdstr, USBDevice>& UpdateUsbMap();
	bool CompareUSBMap();
	unsigned __stdcall ThreadProcessMonit(void* parameter);
	bool IsMsgAIDA(COMMAND c);
public:
	std::mutex MsgLock;
	std::mutex AIDALock;
	std::mutex StartLock;
	bool bStart = false;
	stdstr CPUID;
	
private:
	std::map<stdstr, USBDevice> mUsbmap;
	std::thread * mMonProThread;
	ProcessMonitWMI* pMonit;
	Json::Value StartRoot;
	Json::Value EventRoot;
	Json::Value EndRoot;
	std::set<COMMAND> msgAIDASet;
};
