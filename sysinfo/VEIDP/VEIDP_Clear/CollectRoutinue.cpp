#include"stdafx.h"
#include"CollectRoutinue.h"
#include"CollectMessage.h"


#ifdef _WIN32
#include<direct.h>
#include"../../../../Network/SocketEncode/SocketEncode/base64.h"
#include"../../../Setting/WDesktop/LnkUtils.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Windows/AppManager/AppManager/Apps.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../../USB/WUSBDevice/USBUtils.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#else
#endif


#include"../../../Time/Time/Time.hpp"

#include"Utils.h"
#include"UserMessage.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"
#include"../../../Time/Time/WinTime.h"



void CollectRoutinue::Awake()
{
	LOG_DAILY("CollectRoutinue Awake Begin");

	//电脑开机时间
	GlobalwbUptime = ::GetAnsiTime();

                            //缓存一份，后续不再取
#ifdef _WIN32
    mUsbmap = USBUtils::GetUsbInstanceMap();							//开机时取一份USB设备

    netInfo = SysInfoUtils::GetNetInfo();
	mInvalidVal.Load((PathHelper::GetModuleFolder() + "InvalidVal.txt").c_str());
    mWMISet.insert(COMMAND::COMM_COLLALL);
    mWMISet.insert(COMMAND::COMM_HARDWARE);
#endif



	LOG_DAILY("CollectRoutinue Awake End");
}
bool CollectRoutinue::Start()
{
#ifdef _WIN32
	if (!mWMI.Init())
	{
		LOG(std::string("WMI Init Error : ") + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()));
		ThreadManager::Destroy();
	}
	else
	{
		LOG_DAILY(std::string("WMI Init OK : "));
	}
	{
		//cpuid
		vector<WMICPU> vecCPU;
		std::string exce;
		if (mWMI.Begin() && mWMI.GetCPU(vecCPU, exce))
		{
			if (vecCPU.size() >= 1 && !vecCPU[0].ProcessorId.empty())
			{
				CPUID = vecCPU[0].ProcessorId;
			}
			mWMI.End();
		}
		else
		{
			LOG(std::string("GetCPU Error") + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + exce);
		}
	}
	mSession = WTSGetActiveConsoleSessionId();
	std::string exce;
	//系统补丁
	if (mWMI.Begin() && mWMI.GetPatch(patchvec, exce))
	{
		mWMI.End();
	}
	else
	{
		LOG(std::string("GetPatch Error") + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + exce);
	}
#endif
    TickEnd();
	return true;
}
bool CollectRoutinue::Tick()
{
#ifdef _WIN32
	//获取进程信息
	time_t t = ::GetAnsiTime();
	if (t - mLastMonit >= 3)
	{
		mMonit.MonitProcessOnce(mSession);
		mLastMonit = t;
	}
#endif
	return false;
}
void CollectRoutinue::Destroy()
{
#ifdef _WIN32
	//mWMI.End();
	mWMI.Destroy();
#endif
}
void CollectRoutinue::SendPacket(const Json::Value& rData, int tag, int command, MQ_TYPE target)
{
	auto pMsg = MSG_NEW(Msg_Send2MQ);
	pMsg->rData = rData.toStyledString();
	pMsg->tag = tag;
	pMsg->target = target;
	pMsg->command = command;
	ThreadManager::PushMsg(THREAD_ID::THREAD_USER, pMsg);
}
void CollectRoutinue::OnCommandCollStart(const std::string& orgCode)
{
	if (StartRoot.empty())
	{
#ifdef _WIN32
		StartRoot["wbUptime"] = GlobalwbUptime;							//开机时间
		StartRoot["LoginName"] = CONVERT(SysInfoUtils::GetUser());				//用户名
		WindowsInfo winfo = SysInfoUtils::GetWindowsInfo();
		{
			StartRoot["wbyu"] = CppConvert::UnicodeToUTF8(winfo.langroup);
			StartRoot["ip"] = netInfo.strIp;
			StartRoot["mac"] = netInfo.strMac;
			StartRoot["cpu"] = CONVERT(SysInfoUtils::GetCPU());
			TCHAR os[1024] = { 0 };
			wsprintf(os, _T("%s %s %s"), SysInfoUtils::GetProductName().c_str(),
				SysInfoUtils::GetOSReleaseID().c_str(), SysInfoUtils::GetOSCurrBuild().c_str());
			StartRoot["os"] = CONVERT(os);
			StartRoot["wbHostName"] = CONVERT(SysInfoUtils::GetHostName());
			StartRoot["wbinitializeDate"] = TOSTR(SysInfoUtils::GetOSInstallDate());
			StartRoot["MachineName"] = CONVERT(SysInfoUtils::GetPCName());
		}

		{
			StartRoot["orgCode"] = CONVERT(orgCode);

			Json::Value wbPatch;
			for (int i = 0; i < patchvec.size(); i++)
			{
				Json::Value hotfix;
				hotfix["description"] = CONVERT(patchvec[i].GetDesc());
				hotfix["name"] = CONVERT(patchvec[i].Name);
				hotfix["install_time"] = patchvec[i].GetInstall();
				wbPatch.append(hotfix);
			}
			if (wbPatch.isNull())
			{
				StartRoot["wbPatch"] = "";
			}
			else
			{
				StartRoot["wbPatch"] = wbPatch;
			}

			stdstr SystemProductName;
			stdstr serial_num;
			stdstr BIOSVersion;
			stdstr ComputerProducer;
			
			std::string exce;
			if (mWMI.Begin())
			{
				vector<WMIComputerSystem> csvec;
				if (mWMI.GetComputerSystem(csvec, exce))
				{
					for (int i = 0; i < csvec.size(); i++)
					{
						ComputerProducer = csvec[i].Manufacturer;
					}
				}
				else
				{
					LOG(std::string("GetComputerSystem Error") + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + exce);
				}

				vector<WMIBIOS> biosvec;
				if (mWMI.GetBIOS(biosvec, exce))
				{
					for (int i = 0; i < biosvec.size(); i++)
					{
						if (IsInvalidAIDAValue(serial_num))
						{
							serial_num = biosvec[i].csSerialNum;
						}
						if (IsInvalidAIDAValue(SystemProductName))
						{
							SystemProductName = biosvec[i].Name;
						}
						if (IsInvalidAIDAValue(BIOSVersion))
						{
							BIOSVersion = biosvec[i].SMBIOSBIOSVersion;
						}
					}
				}
				else
				{
					LOG(std::string("GetBIOS Error") + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + exce);
				}

				mWMI.End();
			}

			StartRoot["SystemProductName"] = CONVERT(SystemProductName);
			StartRoot["serial_num"] = CONVERT(serial_num);
			StartRoot["BIOSVersion"] = CONVERT(BIOSVersion);
			StartRoot["wbRegisteredOwner"] = CONVERT(ComputerProducer);
		}


		//错误日志采集
		std::vector<EventLog> eventvec;
		SysInfoUtils::GetEventLog(eventvec);
		Json::Value wberrLog;
		for (auto it = eventvec.begin(); it != eventvec.end(); it++)
		{
			if (it->EventType == EVENTLOG_ERROR_TYPE)
			{
				Json::Value error;
				char str2[64] = { 0 };
				sprintf_s(str2, sizeof(str2), "%d", it->EventID & 0xFFFF);

				error["LogTime"] = to_string(it->TimeGenerated);
				error["LogID"] = str2;
				error["LogTypeName"] = CONVERT(it->GetEventTypeStr());
				wberrLog.append(error);
			}
		}
		if (wberrLog.isNull())
		{
			StartRoot["errorLog"] = "";
		}
		else
		{
			StartRoot["errorLog"] = wberrLog;
		}

		StartRoot["cpuid"] = CPUID;

		//已安装的应用程序
		Json::Value softList;
		vector<RegApplication> info;
		AppManager::GetSoftListInfo(info);
		for (int i = 0; i < info.size(); i++)
		{
			Json::Value soft;
			soft["name"] = CONVERT(info[i].DisplayName);  //info[i].displayName;
			soft["version"] = CONVERT(info[i].DisplayVersion);
			if (info[i].InstallDate.empty())
			{
				soft["Time"] = CONVERT(info[i].InstallDate);
			}
			else
			{
				soft["Time"] = "*";
			}
			soft["Post"] = CONVERT(info[i].Publisher);
			softList.append(soft);
		}

		if (softList.isNull())
		{
			StartRoot["softlist"] = "";
		}
		else
		{
			StartRoot["softlist"] = softList;
		}
#endif
	}
}
void CollectRoutinue::OnCommandCollEnd()
{
	TickEnd();
	SendEnd();
}
//收集全部信息
Json::Value CollectRoutinue::OnCommandCollAll()
{
	Json::Value instructRoot;
	try
	{
		instructRoot["commandType"] = (int)COMMAND::COMM_COLLALL;

		Json::Value instructGlobal;
		instructGlobal.append(instructScreenCapture());
		instructGlobal.append(SendDiskInfo());											//磁盘
		instructGlobal.append(SendMemoryInfo());							//内存
		instructGlobal.append(SendMonitInfo());									//屏幕信息
		instructGlobal.append(SendPrinterInfo());										//打印机
		instructGlobal.append(SendKeyMouse());										//键盘鼠标
		instructGlobal.append(SendCPUInfo());
		instructGlobal.append(SendMotherInfo());
		instructGlobal.append(SendUSB());
		instructRoot["acquisitionData"] = instructGlobal;
		return 	instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return 	instructRoot;
	}
}

//收集全部硬件信息
Json::Value CollectRoutinue::OnCommandHardware()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("OnCommandHardware Begin");
		instructRoot["commandType"] = (int)COMMAND::COMM_HARDWARE;

		Json::Value instructGlobal;
		instructGlobal.append(SendDiskInfo());											//磁盘
		instructGlobal.append(SendMemoryInfo());							//内存
		instructGlobal.append(SendMonitInfo());									//屏幕信息
		instructGlobal.append(SendPrinterInfo());										//打印机
		instructGlobal.append(SendKeyMouse());										//键盘鼠标
		instructGlobal.append(SendCPUInfo());
		instructGlobal.append(SendMotherInfo());
		instructGlobal.append(SendUSB());
		instructRoot["acquisitionData"] = instructGlobal;

		LOG_DAILY("OnCommandHardware End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

//有事件发生，主动发送
Json::Value CollectRoutinue::OnEventCollect()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("OnEventCollect begin");
		BUILD_MSG_HEAD(instructRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_EVENT;

		Json::Value instructGlobal;
		instructGlobal.append(SendUSB());
		instructRoot["acquisitionData"] = instructGlobal;
		LOG_DAILY("OnEventCollect end");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}
void CollectRoutinue::SetVersion(Json::Value data)
{
	try
	{
		auto pMsg = MSG_NEW(Msg_SetVetsion);
		if (!data["versionName"].isNull())
		{
			pMsg->version = data["versionName"].asString();
		}
		if (!data["downloadAddr"].isNull())
		{
			pMsg->url = data["downloadAddr"].asString();
		}
		if (!data["md5"].isNull())
		{
			pMsg->md5 = data["md5"].asString();
		}
		if (!data["updateType"].isNull())
		{
			pMsg->updateType = data["updateType"].asInt();
		}
		ThreadManager::PushMsg(THREAD_ID::THREAD_USER, pMsg);
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void CollectRoutinue::SetOrgcode(Json::Value data)
{
	try
	{
		auto pMsg = MSG_NEW(Msg_SetOrgcode);
		if (!data["orgcode"].isNull())
		{
			pMsg->orgcode = data["orgcode"].asString();
		}
		ThreadManager::PushMsg(THREAD_ID::THREAD_USER, pMsg);
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
//采集截图功能
//以下是读写图片的调用代码：

Json::Value CollectRoutinue::instructScreenCapture()
{
	Json::Value instructRoot;

	try
	{
		LOG_DAILY("instructScreenCapture Begin");
#ifdef _WIN32
		FileManager::CreateFloder(LogUtils::GetMonthDir());
		stdstr picDir = (LogUtils::GetMonthDir() + "pic\\");
		FileManager::CreateFloder(picDir);
		stdstr picName = picDir + WinTimeUtils::GetNowYMD() + "_" + WinTimeUtils::GetNowHMS();
		string path = picName + ".jpg";
		//截图功能 转换base64发送
		// 1 屏幕截图
		GetWindowScreen2(path);

		//弥补：如果写字的存在，则用写字的，否则用原始的
		string textPath = picName + "_TEXT.jpg";
		if (FileManager::IsFileExist(textPath.c_str()))
		{
			FileManager::RemoveFile(path);
			path = textPath;
		}

		// 2 图片转换base64	
		string data;
		ReadPhotoFile(path, data);
		Json::Value wmiScreenCapture;
		wmiScreenCapture["pic"] = data;
		auto str = wmiScreenCapture["pic"].toStyledString();
		if (str.size() < data.size())
		{
			LOG("instructScreenCapture dataSize" + to_string(data.size()) + " But jsonData Size " + to_string(str.size()));
		}
		else
		{
			LOG_DAILY("instructScreenCapture dataSize" + to_string(data.size()) + " JsonData Size " + to_string(str.size()));
		}

		instructRoot["acquisitionData"].append(wmiScreenCapture);
		instructRoot["commandType"] = (int)COMMAND::COMM_SCREEN;
#endif
		LOG_DAILY("instructScreenCapture End");

		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}


Json::Value CollectRoutinue::SendMemoryInfo()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("SendMemoryInfo Begin");
		std::vector<IDPMemory> vec;
		GetIDPMemory(vec);
		for (int i = 0; i < vec.size(); i++)
		{
			Json::Value root;
			auto& info = vec[i];
			root["brand"] = info.brand;
			root["memoryMZ"] = info.memoryMZ;
			root["SerialNumber"] = info.SerialNumber;
			root["series"] = info.series;
			root["size"] = info.size;
			instructRoot["acquisitionData"].append(root);
		}

		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_MEMORY;
		LOG_DAILY("SendMemoryInfo End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

Json::Value CollectRoutinue::SendMonitInfo()
{
	Json::Value instructRoot;
	LOG_DAILY("SendMonitInfo Begin");
	try
	{
		std::string wallpaper;
#ifdef _WIN32
		for (int i = 0; i < 10; i++)
		{
			//屏幕壁纸
			std::wstring wstr;
			auto hr = GetWallpaper(wstr);
			if (hr != S_OK)
			{
				LOG(std::string("GetWallpaper Error ") + ErrorUtils::ComErrorMessage(hr) + ",will use wmi interface");
				std::string exce;
				wallpaper = mWMI.GetDesktopWallpaper(exce);
				if (mWMI.GetLastErrorWMI() != S_OK)
				{
					LOG(std::string("WMI GetWallpaper Error ") + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + exce);
				}
				wallpaper = CONVERT(wallpaper);
			}
			else
			{
				wallpaper = CppConvert::UnicodeToUTF8(wstr);
			}
			if (!wallpaper.empty())
			{
				break;
			}
		}
#endif

		std::vector<IDPMonit> vec;
		GetMonit(vec);

		for (int i = 0; i < vec.size(); i++)
		{
			Json::Value root;
			root["brand"] = CONVERT(vec[i].brand);

			root["imageSize"] = CONVERT(vec[i].imageSize);

			root["size"] = CONVERT(vec[i].size);

			root["model"] = CONVERT(vec[i].model);

			root["SerialNumber"] = CONVERT(vec[i].SerialNumber);

			root["backImage"] = CONVERT(wallpaper);

			instructRoot["acquisitionData"].append(root);
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_MONIT;

		LOG_DAILY("SendMonitInfo End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
        sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

Json::Value CollectRoutinue::SendPrinterInfo()
{
	Json::Value instructRoot;
	try
	{
		std::vector<IDPPrinter> vec;
		GetPrinter(vec);
		for (int i = 0; i < vec.size(); i++)
		{
			Json::Value root;

			root["isDefault"] = CONVERT(vec[i].Default);
			root["driver"] = CONVERT(vec[i].Driver);
			root["name"] = CONVERT(vec[i].Name);
			root["isShare"] = CONVERT(vec[i].Share);
			root["status"] = CONVERT(vec[i].Status);
			instructRoot["acquisitionData"].append(root);
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_PRINTER;
		LOG_DAILY("SendPrinterInfo End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}


Json::Value CollectRoutinue::SendDiskInfo()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("SendDiskInfo Begin");

		std::vector<IDPDisk> diskvec;
		std::vector<IDPDrive> drivec;
		BOOL bRet = GetIDPDisk(drivec, diskvec);
		//统计数据
		Json::Value diskRoot;
		for (int i = 0; i < diskvec.size(); i++)
		{
			auto& info = diskvec[i];
			Json::Value js;
			js["name"] = CONVERT(info.name);
			js["totalSize"] = to_string(info.totalSize);
			js["residueSize"] = to_string(info.residueSize);
#if __TEST_VM
#else
			diskRoot["disk"].append(js);
#endif
		}
		if (diskRoot.isNull())
		{
			diskRoot["disk"] = "";
		}
		instructRoot["acquisitionData"].append(diskRoot);

		Json::Value drivecRoot;
		for (int i = 0; i < drivec.size(); i++)
		{
			auto& info = drivec[i];
			Json::Value js;
			js["totalSize"] = to_string(info.totalSize) + "G";
            js["brand"] = CONVERT(info.brand.c_str());
            js["InterfaceType"] = CONVERT(info.InterfaceType);
            js["rev"] = CONVERT(info.rev);
            js["SerialNumber"] = CONVERT(info.SerialNumber);
#if __TEST_VM
#else
			drivecRoot["drivec"].append(js);
#endif
		}
		if (drivecRoot.isNull())
		{
			drivecRoot["drivec"] = "";
		}
		instructRoot["acquisitionData"].append(drivecRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_DISK;
		LOG_DAILY("SendDiskInfo End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

Json::Value CollectRoutinue::SendCPUInfo()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("SendCPUInfo Begin");

		std::vector<IDPCPU> vec;
		BOOL bRet = GetIDPCPU(vec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetIDPCPU Error");
		}

		//统计数据
		for (int i = 0; i < vec.size(); i++)
		{
			auto& info = vec[i];
			Json::Value js;
			js["brand"] = CONVERT(info.brand);
			js["series"] = CONVERT(info.series);
			js["cpuT"] = CONVERT(info.cpuT);
			js["cpuFanR"] = CONVERT(info.cpuFanR);
			js["Tjmax"] = CONVERT(info.Tjmax);
			js["cpuid"] = CONVERT(info.cpuid);
#if __TEST_VM
#else
			instructRoot["acquisitionData"].append(js);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_CPU;
		LOG_DAILY("SendCPUInfo End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}
Json::Value CollectRoutinue::SendMotherInfo()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("SendMotherInfo Begin");
		std::vector<IDPMother> vec;
		BOOL bRet = GetIDPMother(vec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetIDPMother Error");
		}
		//统计数据
		for (int i = 0; i < vec.size(); i++)
		{
			auto& info = vec[i];
			Json::Value js;
			js["brand"] = CONVERT(info.brand);
			js["series"] = CONVERT(info.series);
			js["name"] = CONVERT(info.name);
			js["mainboardCore"] = CONVERT(info.mainboardCore);
			js["busBandwidth"] = CONVERT(info.busBandwidth);
#if __TEST_VM
#else
			instructRoot["acquisitionData"].append(js);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_MAINBOARD;
		LOG_DAILY("SendMotherInfo End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

Json::Value CollectRoutinue::SendKeyMouse()
{
	Json::Value instructRoot;
	try
	{
		LOG_DAILY("SendKeyMouse Begin");

		std::vector<IDPKeyboard> keyVec;
		BOOL bRet = GetKeyboard(keyVec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetKeyboard Error");
		}
		std::vector<IDPMouse> mouseVec;
		bRet = GetMouse(mouseVec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetMouse Error");
		}
		//统计数据
		Json::Value mouseRoot;
		for (int i = 0; i < mouseVec.size(); i++)
		{
			auto& info = mouseVec[i];
			Json::Value js;
			js["name"] = CONVERT(info.name);
			js["isExistRoller"] = CONVERT(info.isExistRoller);
			js["rollingLine"] = to_string(info.rollingLine);
#if __TEST_VM
#else
			mouseRoot["mouse"].append(js);
#endif
		}
		if (mouseRoot.isNull())
		{
			mouseRoot["mouse"] = "";
		}
		instructRoot["acquisitionData"].append(mouseRoot);

		Json::Value keyRoot;
		for (int i = 0; i < keyVec.size(); i++)
		{
			auto& info = keyVec[i];
			Json::Value js;
			js["name"] = CONVERT(info.name);
			js["type"] = CONVERT(info.type);
			js["keyboardInterface"] = CONVERT(info.keyboardInterface);
#if __TEST_VM
#else
			keyRoot["keyboard"].append(js);
#endif
		}
		if (keyRoot.isNull())
		{
			keyRoot["keyboard"] = "";
		}
		instructRoot["acquisitionData"].append(keyRoot);

		instructRoot["commandType"] = (int)COMMAND::COMM_KEY_MOUSE;
		LOG_DAILY("SendKeyMouse End");
		return instructRoot;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

Json::Value CollectRoutinue::SendUSB()
{
	Json::Value instructRoot;
	//统计数据
	try
	{
		LOG_DAILY("SendUSB Begin");
		Json::Value usbRoot;
		//因为每次发生变动都会更新缓存，所以这里直接取缓存的就可以
#ifdef _WIN32
		for (auto it = mUsbmap.begin(); it != mUsbmap.end(); it++)
		{

			Json::Value js;
			js["description"] = CONVERT(it->second.Description);
			js["busReported"] = Convert::UnicodeToUtf8(it->second.BusReportedDeviceDescription);
#if __TEST_VM
#else
			usbRoot["usb"].append(js);
#endif
	}
#endif
		if (usbRoot.isNull())
		{
			usbRoot["usb"] = "";
		}
		instructRoot["acquisitionData"].append(usbRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_USB;
		LOG_DAILY("SendUSB End");
		return instructRoot;
}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		return instructRoot;
	}
}

Json::Value CollectRoutinue::TickColllect()
{
	Json::Value RunTimeRoot;
	RunTimeRoot["wbUptime"] = GlobalwbUptime;

	//开机运行时长
#ifdef _WIN32
	RunTimeRoot["wbRunUptime"] = to_string(WinTimeUtils::GetBootRunTimeS());

	//获取TCP监听端口
	{
		std::vector<TCPPort> vec;
		SysInfoUtils::GetTCPportlist(vec);
		Json::Value TcpSoftList;
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			Json::Value tcp;
			tcp["Local Address"] = it->GetIpPortStr();
			//tcp["Foreign Address"] = usRemotePortSendBuf;
			tcp["State"] = it->GetStateStr();
			tcp["PID"] = to_string(it->dwOwningPid);
			TcpSoftList.append(tcp);
		}
		if (TcpSoftList.isNull())
		{
			RunTimeRoot["wbTCP"] = "";
		}
		else
		{
			RunTimeRoot["wbTCP"] = TcpSoftList;
		}
	}


	//获取UDP监听端口
	{
		std::vector<UDPPort> vec;
		SysInfoUtils::GetUDPportlist(vec);
		Json::Value UdpSoftList;
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			Json::Value udp;
			udp["Local Address"] = it->GetIpPortStr();
			//tcp["Foreign Address"] = usRemotePortSendBuf;
			udp["State"] = "*";
			udp["PID"] = to_string(it->dwOwningPid);
			UdpSoftList.append(udp);
		}
		if (UdpSoftList.isNull())
		{
			RunTimeRoot["wbUDP"] = "";
		}
		else
		{
			RunTimeRoot["wbUDP"] = UdpSoftList;
		}
	}

	{
		Json::Value RetPro;
		std::map<DWORD, ProcessInfo> mapProces;
		ProcessHelper::GetProcessList(mapProces);
		for (auto it = mapProces.begin(); it != mapProces.end(); it++)
		{
			Json::Value PidPro;
			PidPro["Pid"] = to_string(it->first);
			PidPro["Pro"] = CONVERT(it->second.szExeFile);
			RetPro.append(PidPro);
		}
		//获取已运行的进程
		RunTimeRoot["runPRO"] = RetPro;
	}


	//操作系统+版本	"Windows 10 Pro 1903 18362",
	string wbHostInfo1 = SysInfoUtils::GetOSName();

	if (!(strncmp(wbHostInfo1.c_str(), "Windows", 7)))
	{
		wbHostInfo1 = "Windows";
	}
	else if (!(strncmp(wbHostInfo1.c_str(), "windows", 7)))
	{
		wbHostInfo1 = "windows";
	}
    RunTimeRoot["os"] = wbHostInfo1;
#endif

	return RunTimeRoot;
}
void CollectRoutinue::TickEnd()
{
	try
	{
		LOG_DAILY("TickEnd Begin");
		//系统补丁
#ifdef _WIN32
		std::string exce;
		vector<WMIPatchInfo> patchvec;
		if (mWMI.Begin() && mWMI.GetPatch(patchvec, exce))
		{
			Json::Value wbPatch;
			for (int i = 0; i < patchvec.size(); i++)
			{
				Json::Value hotfix;
				hotfix["description"] = CONVERT(patchvec[i].GetDesc());//hotfixInfo[i].description;
				hotfix["name"] = CONVERT(patchvec[i].Name);// hotfixInfo[i].name;
				hotfix["install_time"] = patchvec[i].GetInstall();
				wbPatch.append(hotfix);
			}
			if (wbPatch.isNull())
			{
				EndRoot["wbPatch"] = "";
			}
			else
			{
				EndRoot["wbPatch"] = wbPatch;
			}
			mWMI.End();
		}
		else
		{
			LOG("WMI GetPatch Error	" + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + "	" + exce);
		}



		//错误日志采集
		std::vector<EventLog> eventvec;
		SysInfoUtils::GetEventLog(eventvec);
		Json::Value wberrLog;
		for (auto it = eventvec.begin(); it != eventvec.end(); it++)
		{
			if (it->EventType == EVENTLOG_ERROR_TYPE)
			{
				Json::Value js;
				char str2[64] = { 0 };
				sprintf(str2, "%d", it->EventID & 0xFFFF);
				js["LogTime"] = std::to_string(it->TimeGenerated);
				js["LogID"] = str2;
				js["LogTypeName"] = CONVERT(it->GetEventTypeStr());
				wberrLog.append(js);
			}
		}
		if (wberrLog.isNull())
		{
			EndRoot["errorLog"] = "";
		}
		else
		{
			EndRoot["errorLog"] = wberrLog;
		}
		//已安装的应用程序
		Json::Value softList;
		vector<RegApplication> info;
		AppManager::GetSoftListInfo(info);
		for (int i = 0; i < info.size(); i++)
		{
			Json::Value soft;
			soft["name"] = CONVERT(info[i].DisplayName);  //info[i].displayName;
			soft["version"] = CONVERT(info[i].DisplayVersion);
			if (info[i].InstallDate != "*")
			{
				soft["Time"] = CONVERT(SetTime(info[i].InstallDate));
			}
			else
			{
				soft["Time"] = CONVERT(info[i].InstallDate);
			}
			soft["Post"] = CONVERT(info[i].Publisher);
			softList.append(soft);
		}

		if (softList.isNull())
		{
			EndRoot["softlist"] = "";
		}
		else
		{
			EndRoot["softlist"] = softList;
		}
#endif
		LOG_DAILY("TickEnd End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
        sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void CollectRoutinue::SendEnd()
{
	//=============== 关机发送的数据 加上关机心跳预存的数据 ===============

	EndRoot["mac"] = netInfo.strMac;
	//使用时间戳发送
	EndRoot["Endtime"] = to_string(::GetAnsiTime());
	EndRoot["wbUptime"] = GlobalwbUptime;

	//=========================进程统计=====================
#ifdef _WIN32
	auto countMap = mMonit.GetCountMap();
	Json::Value procList;
	//拿到name和计数
	for (auto it = countMap.begin(); it != countMap.end(); it++)
	{
		Json::Value procInfo;
		procInfo["name"] = CONVERT(it->first.c_str());
		procInfo["counter"] = to_string(it->second);
		procList.append(procInfo);
	}
	if (procList.isNull())
	{
		EndRoot["softwareCount"] = "";
	}
	else
	{
		EndRoot["softwareCount"] = procList;
	}
#endif
	SendPacket(EndRoot, TAG_END, COMMAND::COMM_EXIT, MQ_LOCAL);
}
bool CollectRoutinue::IsInvalidAIDAValue(const string& val)
{
	//find
	if (val.empty())
	{
		return true;
	}
	const DATA_MAP& tData = mInvalidVal.GetData();
	for (auto it = tData.begin(); it != tData.end(); it++)
	{
		//逐个匹配
		const DATA& row = it->second;
		//模式匹配
		if (row["matchStyle"] == "1")
		{
			if (val.find(row["Str"]) != string::npos)
			{
				return true;
			}
		}
		//全匹配
		else if (row["matchStyle"] == "2")
		{
			if (val == row["Str"])
			{
				return true;
			}
		}
	}
	return false;
}
void CollectRoutinue::ShowMessageBox(const Msg_CommandCollect& rMsg)
{
	try
	{
		Json::Value root;
		if (JsonTools::SetJsonData(rMsg.msg.c_str(), root))
		{
			Json::Value data = root["data"];
			if (data.isNull())
			{
				return;
			}
			MsgBox *msg = new MsgBox();
			if (!data["message"].isNull())
			{
				std::string str = data["message"].asString();
				msg->Message = str;
			}
			if (!data["title"].isNull())
			{
				std::string str = data["title"].asString();
				msg->Title = str;
			}
			if (!msg->Message.empty())
			{
				auto func = [=](void* argv)->void*
				{
					MsgBox* msgBox = static_cast<MsgBox *>(argv);
					if (msgBox)
					{
#ifdef _WIN32
						MessageBoxA(NULL, msgBox->Message.c_str(), msg->Title.c_str(), MB_OK);
#endif
						delete msgBox;
						msgBox = nullptr;
						return (void*)1;
					}
					return 0;
				};
				ThreadManager::PushAsyncTask(func, msg);
			}
		}
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
        sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void CollectRoutinue::ShowMessageBox(Json::Value data)
{
	try
	{
		MsgBox *msg = new MsgBox();
		if (!data["message"].isNull())
		{
			std::string str = data["message"].asString();
			msg->Message = str;
		}
		if (!data["title"].isNull())
		{
			std::string str = data["title"].asString();
			msg->Title = str;
		}
		if (!msg->Message.empty())
		{
			auto func = [=](void* argv)->void*
			{
				MsgBox* msgBox = static_cast<MsgBox *>(argv);
				if (msgBox)
				{
#ifdef _WIN32
					MessageBoxA(NULL, msgBox->Message.c_str(), msg->Title.c_str(), MB_OK | MB_TOPMOST);
#endif
					delete msgBox;
					msgBox = nullptr;
					return (void*)1;
				}
				return 0;
			};
			ThreadManager::PushAsyncTask(func, msg);
		}
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
        sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

bool CollectRoutinue::IsMsgWMI(int c)
{
	if (mWMISet.find(c) != mWMISet.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
