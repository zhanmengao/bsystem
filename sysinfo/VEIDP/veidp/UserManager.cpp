#include"stdafx.h"
#include"UserManager.h"
#include"IDPEvent.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Time/Time/Time.hpp"
#include"../../../Time/Time/WinTime.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../../Windows/AppManager/AppManager/Apps.h"
#include"../../SystemInfo/WMI/WMIUtils.h"
#include"../../../../Network/Serialization/Json/JsonTools.hpp"
#include"veidp.h"




//进程启动
void UserManager::Start()
{
	//启动进程监控
	pMonit = new ProcessMonitWMI(&IDPManager::WMI());
	mMonProThread = new thread(&UserManager::ThreadProcessMonit, this, pMonit);
	msgAIDASet.insert(COMMAND::COMM_COLLALL);
	msgAIDASet.insert(COMMAND::COMM_DISK);
	msgAIDASet.insert(COMMAND::COMM_MEMORY);
	msgAIDASet.insert(COMMAND::COMM_MONIT);
	msgAIDASet.insert(COMMAND::COMM_PRINTER);
	msgAIDASet.insert(COMMAND::COMM_KEY_MOUSE);
	msgAIDASet.insert(COMMAND::COMM_HARDWARE);
	msgAIDASet.insert(COMMAND::COMM_CPU);
	msgAIDASet.insert(COMMAND::COMM_MAINBOARD);
}
std::map<stdstr, DWORD> UserManager::MonitProcessEnd()
{
	pMonit->SetUpdate(false);
	return pMonit->GetCountMap();
}
void UserManager::Destroy()
{
	if (mMonProThread)
	{
		if (mMonProThread->joinable())
		{
			mMonProThread->join();
		}
		delete mMonProThread;
		mMonProThread = nullptr;
	}
	if (pMonit)
	{
		delete pMonit;
		pMonit = nullptr;
	}
}
//心跳发送数据
void  UserManager::TimerCall()
{
	Json::Value RunTimeRoot;
	{
		//当前时间
		RunTimeRoot["wbUptime"] = std::to_string(::GetAnsiTime());
		//开机运行时长
		RunTimeRoot["wbRunUptime"] = std::to_string(WinTimeUtils::GetBootRunTimeS());
	}

	//网卡信息
	{
		net_info netInfo = SysInfoUtils::GetNetInfo();
		RunTimeRoot["ip"] = netInfo.strIp;
		RunTimeRoot["mac"] = netInfo.strMac;
	}
	//TCP
	{
		Json::Value TcpList;
		std::vector<TCPPort> vec;
		SysInfoUtils::GetTCPportlist(vec);
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			Json::Value item;
			item["Local Address"] = it->GetIpPortStr();
			item["State"] = it->GetStateStr();
			item["PID"] = to_string(it->dwOwningPid);
			TcpList.append(item);
		}
		RunTimeRoot["wbTCP"] = TcpList;
	}
	//UDP
	{
		Json::Value UdpList;
		std::vector<UDPPort> vec;
		SysInfoUtils::GetUDPportlist(vec);
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			Json::Value item;
			item["Local Address"] = it->GetIpPortStr();
			item["State"] = "*";
			item["PID"] = to_string(it->dwOwningPid);
			UdpList.append(item);
		}
		RunTimeRoot["wbUDP"] = UdpList;
	}
	//进程列表
	{
		Json::Value ProList;
		std::map<DWORD, ProcessInfo> promap;
		ProcessHelper::GetProcessList(promap);
		for (auto it = promap.begin(); it != promap.end(); it++)
		{
			Json::Value item;
			item["Pid"] = to_string(it->second.th32ProcessID);
			item["Pro"] = it->second.szExeFile;
			ProList.append(item);
		}
		RunTimeRoot["runPRO"] = ProList;
	}

}




//发送全部数据
void  UserManager::SendALL()
{
	Json::Value instructRoot;
	//MAC
	{
		auto netinfo = SysInfoUtils::GetNetInfo();
		instructRoot["mac"] = netinfo.strMac;
	}
	//时间
	{
		instructRoot["acquisitionTime"] = std::to_string(::GetAnsiTime());
		instructRoot["commandType"] = 1;
	}

	//截屏
	{

	}
}
int UserManager::ProcessMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt)
{
	//消息解包
	Json::Value  packet;
	int command = -1;												//命令
	string mac;

	if (JsonTools::SetJsonData(GetMessageBody(msgExt), packet))
	{
		try
		{
			if (!packet["commandType"].isNull())
			{
				command = packet["commandType"].asInt();
			}

			if (!packet["mac"].isNull())
			{
				mac = packet["mac"].asString();
			}
		}
		catch (const std::exception&)
		{
			printf("MsgString to jsonObject Error\n");
		}
	}
	//检测是否给我发
	auto netinfo = SysInfoUtils::GetNetInfo();
	if (netinfo.strMac != mac)
	{
		return E_CONSUME_SUCCESS;
	}
	return E_CONSUME_SUCCESS;
}
const std::map<stdstr, USBDevice>& UserManager::GetUsbMap() const
{
	return mUsbmap;
}
std::map<stdstr, USBDevice>& UserManager::UpdateUsbMap()
{
	mUsbmap = USBUtils::GetUsbInstanceMap();
	return mUsbmap;
}
//用最新的和他比较
bool UserManager::CompareUSBMap()
{
	auto current = USBUtils::GetUsbInstanceMap();
	bool bRet = true;
	//大小不同，直接发
	if (current.size() != mUsbmap.size())
	{
		bRet = false;
	}
	else
	{
		for (auto it = current.begin(); it != current.end(); it++)
		{
			auto fit = mUsbmap.find(it->first);
			//在老集合找不到，直接发
			if (fit == mUsbmap.end())
			{
				bRet = false;
				break;
			}
			//和老集合的不同，直接发
			if (fit->second != it->second)
			{
				bRet = false;
				break;
			}
		}
	}

	mUsbmap = current;
	return bRet;
}

unsigned __stdcall UserManager::ThreadProcessMonit(void* parameter)
{
	try
	{
		ProcessMonitWMI* monit = static_cast<ProcessMonitWMI*>(parameter);
		if (monit != NULL)
		{
			LOG_DAILY("MonitProcess Begin");
			monit->MonitProcess(SessionManager::GetSessionID());
			LOG_DAILY("MonitProcess End");
			_endthreadex(1);
			return 1;
		}
		_endthreadex(0);
		return 0;
	}
	catch (const exception& ex)
	{
		LOG(string("Process Monit throw a exception") + ex.what());
		return 0;
	}
}
bool UserManager::IsMsgAIDA(COMMAND c)
{
	if (msgAIDASet.find(c) != msgAIDASet.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
