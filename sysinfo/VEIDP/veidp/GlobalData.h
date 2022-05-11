#pragma once
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include"../../../File/FileManager/FileManager/FileManager.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Thread/BlockQueue/BlockQueue/BlockQueue.hpp"
#include"../../../../C++space/LogManager/LogManager/LogManager.hpp"
#include"../../SystemInfo/WMI/WMIUtils.h"

#include"ConfigManager.h"
#include"UserManager.h"
#include<mutex>
class IDPManager
{
public:
	static void Start();
	static void Update();
	static void Destroy();
private:
	static void Reload();
public:
	static UserManager User;
	static LogManager Log;
	static ConfigManager Config;

	static tm mLastDrivetm;
	static bool bSendEndMessage;
	static time_t mLastAIDA;
private:
	static WMIUtils wmi;
	static std::mutex mWMILock;
public: 
	static WMIUtils& WMI();
};

