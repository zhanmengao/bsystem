#include"stdafx.h"
#include"UserRoutinue.h"
#include "VEIDP_NEW.h"
#include"UserMessage.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"

#include"../../../IPC/ListenerProject/ListenerProject/MessageUtils.h"
#include"CollectMessage.h"

void UserRoutinue::ProcessMsg(MsgBase &rMsg)
{
	char buffer[1024] = { 0 };
	sprintf_s(buffer, "UserRoutinue recv msgid : %u", rMsg.GetId());
	LOG_DAILY(buffer);
	if (rMsg.GetId() == MSG_CREATE)
	{
		auto pChildMsg = dynamic_cast<Msg_Create*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_SEND_MQ)
	{
		auto pChildMsg = dynamic_cast<Msg_Send2MQ*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_RECV_MQ)
	{
		auto pChildMsg = dynamic_cast<Msg_RecvMQ*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_SET_VERSION)
	{
		auto pChildMsg = dynamic_cast<Msg_SetVetsion*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
	else if (rMsg.GetId() == MSG_SET_ORGCODE)
	{
		auto pChildMsg = dynamic_cast<Msg_SetOrgcode*>(&rMsg);
		if (pChildMsg != nullptr)
		{
			HandleMsg(*pChildMsg);
		}
	}
}
void UserRoutinue::HandleMsg(const Msg_Create& rMsg)
{

}
void UserRoutinue::HandleMsg(const Msg_Send2MQ& rMsg)
{
	try
	{
		//如果是结束消息，直接发，不走异步
		if (rMsg.tag == TAG_DEF::TAG_END)
		{
			SendPacketSync(rMsg);
		}
		else
		{
			Msg_Send2MQ *msg = new Msg_Send2MQ(rMsg);
			std::function<void*(void*)> f = std::bind(&UserRoutinue::SendPacketAsync, this, placeholders::_1);
			ThreadManager::PushAsyncTask(f, msg);
		}
	}
	catch (exception& e)
	{
		LOG(e.what());
	}	
}
void UserRoutinue::HandleMsg(const Msg_RecvMQ& rMsg)
{
	try
	{
		LOG_DAILY("ProcessMQMessage Begin");
		Json::Value  resp;
		int mess = 0;
		string mac;
		std::string msg  = rMsg.msg;
		if (JsonTools::SetJsonData(msg.c_str(), resp))
		{
			if (!resp["commandType"].isNull())
			{
				mess = resp["commandType"].asInt();
			}

			if (!resp["mac"].isNull())
			{
				mac = resp["mac"].asString();
			}
		}
		else
		{
			LOG(string("ProcessMQMessage recv a error json \n") + msg);
			mess = COMMAND::COMM_INVALID;
		}
		
		if (mess<=COMMAND::COMM_INVALID || mess>=COMMAND::COMMAND_MAX)
		{
			LOG(string("ProcessMQMessage recv error mess \n") + msg);
			return;
		}
		//是我的消息，才处理
		if (mess == COMMAND::COMM_INVALID ||  mConfig.Mac == mac)
		{
			Msg_CommandCollect *nMsg = new Msg_CommandCollect();
			nMsg->command = mess;
			nMsg->rData = resp;
			nMsg->src = rMsg.src;
			nMsg->msg = msg;
			if (IsMsgAIDA((COMMAND)mess))
			{
				std::function<void*(void*)> f = std::bind(&UserRoutinue::OnCommandCallAIDA, this, placeholders::_1);
				ThreadManager::PushAsyncTask(f, nMsg);
			}
			else
			{
				//解析完毕，将消息发给收集线程
				auto pMsg = MSG_NEW_POINT(Msg_CommandCollect, nMsg);
				ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
			}
		}
		else
		{
			LOG(string("ProcessMQMessage recv error mac \n") + msg);
		}
		LOG_DAILY("User ProcessMQMessage End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void UserRoutinue::HandleMsg(const Msg_SetVetsion& rMsg)
{
	mConfig.SaveVersion(rMsg.version, rMsg.url, rMsg.md5,rMsg.updateType);
}
void UserRoutinue::HandleMsg(const Msg_SetOrgcode& rMsg)
{
	mConfig.SaveOrgcode(rMsg.orgcode);
}