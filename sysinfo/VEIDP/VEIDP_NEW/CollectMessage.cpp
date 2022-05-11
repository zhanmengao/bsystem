#include"stdafx.h"
#include"CollectRoutinue.h"
#include"../../../Time/Time/Time.hpp"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Windows/AppManager/AppManager/Apps.h"
#include"Utils.h"
#include"UserMessage.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"
#include"../../../Time/Time/WinTime.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
void CollectRoutinue::ProcessMsg(MsgBase &rMsg)
{
	char buffer[1024] = { 0 };
	sprintf_s(buffer, "CollectRoutinue recv msgid : %u", rMsg.GetId());
	LOG_DAILY(buffer);
	if (rMsg.GetId() == MSG_CREATE_COLLECT)
	{
		auto pChildMsg = dynamic_cast<Msg_CreateCollect*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_PANT_COLLECT)
	{
		auto pChildMsg = dynamic_cast<Msg_PantCollect*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_COMMAND_COLLECT)
	{
		auto pChildMsg = dynamic_cast<Msg_CommandCollect*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_END_CACHE)
	{
		auto pChildMsg = dynamic_cast<Msg_EndCache*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_EXIT)
	{
		auto pChildMsg = dynamic_cast<Msg_Exit*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_EVENT_COLLECT)
	{
		auto pChildMsg = dynamic_cast<Msg_EventCollect*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
}

//获取开机消息，发回去
void CollectRoutinue::HandleMsg(const Msg_CreateCollect& rMsg)
{
	OnCommandCollStart(rMsg.orgCode);
	SendPacket(StartRoot, TAG_START,COMMAND::COMM_START,MQ_LOCAL);
}

//采集心跳信息，发回去
void CollectRoutinue::HandleMsg(const Msg_PantCollect& rMsg)
{
	try
	{
		LOG_DAILY("HandleMsg Msg_PantCollect");
		//=============== 心跳发送的数据 ===============
		Json::Value RunTimeRoot = TickColllect();
		BUILD_MSG_HEAD(RunTimeRoot);
		//通知User
		SendPacket(RunTimeRoot,TAG_PANT,COMMAND::COMM_TICK,MQ_LOCAL);
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

//指令消息采集
void CollectRoutinue::HandleMsg(const Msg_CommandCollect& rMsg)
{
	Json::Value root;
	TAG_DEF tag = TAG_DEF::TAG_START;
	bool bSend = true;
	if (IsMsgWMI(rMsg.command))
	{
		mWMI.Begin();
	}

	switch (rMsg.command)
	{
	case COMMAND::COMM_INVALID:
	case COMMAND::COMM_COLLALL:
	{
		root = OnCommandCollAll();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_SCREEN:
	{
		root = instructScreenCapture();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_DISK:
	{
		root = SendDiskInfo();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_MEMORY:
	{
		root = SendMemoryInfo();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_MONIT:
	{
		root = SendMonitInfo();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_PRINTER:
	{
		root = SendPrinterInfo();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_KEY_MOUSE:
	{
		root = SendKeyMouse();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_VERSION:
	{
		if (!rMsg.rData["data"].isNull())
		{
			SetVersion(rMsg.rData["data"]);
		}
		bSend = false;
		break;
	}
	case COMMAND::COMM_ORGCODE:
	{
		if (!rMsg.rData["data"].isNull())
		{
			SetOrgcode(rMsg.rData["data"]);
		}
		bSend = false;
		break;
	}
	case COMMAND::COMM_HARDWARE:
	{
		root = OnCommandHardware();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_CPU:
	{
		root = SendCPUInfo();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_MAINBOARD:
	{
		root = SendMotherInfo();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_USB:
	{
		root = SendUSB();
		tag = TAG_INSTRUCT;
		break;
	}
	case COMMAND::COMM_START:
	{
		root = StartRoot;
		tag = TAG_START;
		break;
	}
	case COMMAND::COMM_TICK:
	{
		root = TickColllect();
		tag = TAG_PANT;
		break;
	}
	case COMMAND::COMM_MSGBOX:
	{
		if (!rMsg.rData["data"].isNull())
		{
			ShowMessageBox(rMsg.rData["data"]);
		}
		bSend = false;
		break;
	}
	case COMMAND::COMM_CLOSE:
	{
		//运行脚本
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("stop.bat"), _T(""));
		break;
	}
	}
	if (IsMsgWMI(rMsg.command))
	{
		mWMI.End();
	}
	if (bSend)
	{
		BUILD_MSG_HEAD(root);
		SendPacket(root, tag, rMsg.command, rMsg.src);
	}
}

//关机数据采集
void CollectRoutinue::HandleMsg(const Msg_EndCache& rMsg)
{
	TickEnd();
}

//关机数据发送
void CollectRoutinue::HandleMsg(const Msg_Exit& rMsg)
{
	SendEnd();
}

//发生事件，采集消息
void CollectRoutinue::HandleMsg(const Msg_EventCollect& rMsg)
{
	bool bCompare = false;
	if (!CompareUSBMap())
	{
		bCompare = true;
	}
	//出现不同，发消息
	if (bCompare)
	{
		Json::Value root = OnEventCollect();
		SendPacket(root, TAG_EVENT,COMMAND::COMM_EVENT,MQ_LOCAL);
	}
}