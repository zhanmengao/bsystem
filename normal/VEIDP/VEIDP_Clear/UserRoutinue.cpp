#ifdef _WIN32
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#endif
#include"stdafx.h"
#include"UserRoutinue.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"
#include"CollectMessage.h"
#define CACHE_FILE "PacketCache"
void UserRoutinue::Awake()
{
	try
	{
		LOG_DAILY("UserRoutinue Awake Begin");
        mConfig.ReloadConfig();
#ifdef _WIN32
		RockMQ local;
		local.Produce.SetGroup(mConfig.Group);
		local.Produce.SetIP(mConfig.IP);
		local.Produce.SetTopic(mConfig.Topic);
		local.Produce.SetTimeout(1000);
		local.Produce.SetLogPath(LogUtils::GetMonthDir());

		local.Consu.SetIP(mConfig.InstructIp);
		local.Consu.SetGroup(mConfig.Group);
		local.Consu.SetTopic(mConfig.InstructTopic);
		local.Consu.SetCallBack(ProcessLocalMQMessage);
		local.Consu.SetLogPath(LogUtils::GetMonthDir());

		mMQ.insert(std::make_pair(MQ_LOCAL, local));
		LOG_DAILY("MQList Insert MQ_LOCAL . Produce:" + local.Produce.GetIP() + " " + local.Produce.GetGroup() + 
			"Consu : " + local.Consu.GetIP() + " " + local.Consu.GetGroup());

		RockMQ wan;
		wan.Produce.SetIP(mConfig.wanProduceIp);
		wan.Produce.SetTopic(mConfig.Topic);
		wan.Produce.SetGroup(mConfig.wanGroup);
		wan.Produce.SetTimeout(1000);
		wan.Produce.SetLogPath(LogUtils::GetMonthDir());

		wan.Consu.SetLogPath(LogUtils::GetMonthDir());
		wan.Consu.SetIP(mConfig.wanConsuIp);
		wan.Consu.SetTopic(mConfig.wanConsuTopic);
		wan.Consu.SetGroup(mConfig.wanGroup);
		wan.Consu.SetCallBack(ProcessWanMQMessage);

		mMQ.insert(std::make_pair(MQ_WAN, wan));
		LOG_DAILY("MQList Insert MQ_WAN . Produce:" + wan.Produce.GetIP() + " " + wan.Produce.GetGroup() +
			"Consu : " + wan.Consu.GetIP() + " " + wan.Consu.GetGroup());
#endif
		mTags[TAG_START] = "start";
		mTags[TAG_END] = "end";
		mTags[TAG_PANT] = "pant";
		mTags[TAG_INSTRUCT] = "instruct";
		mTags[TAG_EVENT] = "event";
		mTags[TAG_CACHE] = "cache";

		//load错误文件
		ReadCacheFile();
		LOG_DAILY("UserRoutinue Awake End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s Msg_Send2MQ: %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
bool UserRoutinue::Start()
{
#ifdef _WIN32
	//开机时发送一次采集数据
	auto pMsg = MSG_NEW(Msg_CreateCollect);
	pMsg->orgCode = mConfig.organization;
	ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);

	//开始处理消息
	for (auto it = mMQ.begin(); it != mMQ.end(); it++)
	{
		auto &Consu = it->second.Consu;
		if (Consu.Invalid())
		{
			if (Consu.Sub(mConfig.Mac.c_str()) == _CStatus_::OK)
			{
				LOG_DAILY(_T("StartPushConsumer Sec    group:" + Consu.GetGroup() + "    ip" + Consu.GetIP() +
					"    Topic " + Consu.GetTopic()));
			}
			else
			{
				LOG_DAILY(_T("StartPushConsumer Fail    group:" + Consu.GetGroup() + "    ip" + Consu.GetIP() +
					"    Topic " + Consu.GetTopic()));
			}
		}
	}
#else
#endif
	return true;
}
bool UserRoutinue::Tick()
{
	return false;
}
void UserRoutinue::Destroy()
{
#ifdef _WIN32
	for (auto it = mMQ.begin(); it != mMQ.end(); it++)
	{
		auto &Consu = it->second.Consu;
		Consu.Stop();
		auto &Produce = it->second.Produce;
		Produce.Stop();
	}
#endif
}

void UserRoutinue::SendPacketSync(const Msg_Send2MQ& rMsg)
{
	try
	{
		bool bIsEnd = false;							//是否是关机消息
		if (rMsg.tag == TAG_DEF::TAG_END)
		{
			bIsEnd = true;
			//将关机消息直接缓存
			CachePacket(rMsg);
		}
		if (rMsg.tag >= TAG_MAX || rMsg.tag <0)
		{
			char buffer[1024] = { 0 };
			sprintf(buffer, "%s tag(%d) >= TAG_MAX(%d)", __FUNCTION__,rMsg.tag,TAG_MAX);
			LOG(buffer);
			return;
		}
		int sendTarget = rMsg.target;
		bool bSend = false;
		do
		{
			bSend = SendPacketSync(rMsg.rData.c_str(), mTags[rMsg.tag], sendTarget);
			//如果发成功了，清空缓存
			if (bSend)
			{
				if (bIsEnd)
				{
					DelCachePacket(rMsg.command);
				}
				if (!mCache.empty())
				{
					//组织17号数据
					Json::Value root;

					root["commandType"] = (int)COMMAND::COMM_CACHE;
					std::unique_lock<std::mutex> lock(mCacheLock);
					for (auto it = mCache.begin(); it != mCache.end(); it++)
					{
						Json::Value item;
						Json::Value data;
						auto& info = it->second;
						//先构造Json
						if (JsonTools::SetJsonData(info.str.string, data))
						{
							item["acquisitionData"] = data;
							item["commandType"] = it->second.command;
							root["acquisitionData"].append(item);
						}
					}
					if (root["acquisitionData"].isNull())
					{
						root["acquisitionData"].append("");
					}

					bool bCacheSend = SendPacketSync(root.toStyledString(), "cache", sendTarget);
					if (bCacheSend)
					{
						//发送成功，删除

						mCache.clear();
						WriteCacheFile();
					}
				}
			}
			//发失败了
			else
			{
				if (sendTarget == MQ_TYPE::MQ_WAN)
				{
					//缓存消息，退出循环
					CachePacket(rMsg);
					break;
				}
				else
				{
					sendTarget = MQ_TYPE::MQ_WAN;
				}
			}
		} while (!bSend);
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s Msg_Send2MQ: %s \n", __FUNCTION__, e.what());
		LOG(buffer);
		CachePacket(rMsg);
	}
}
bool UserRoutinue::SendPacketSync(const std::string& packet, const char* tag, int mq)
{

	try
	{
#ifdef _WIN32
		auto it = mMQ.find(mq);
		if (it != mMQ.end())
		{
			auto &Produce = it->second.Produce;
			if (strcmp(tag, "end") == 0)
			{
				//Produce.SetTimeout(500);
			}
			LOG_DAILY(_T("SendMessageSync begin    group:" + Produce.GetGroup() + "    ip" + Produce.GetIP() +
				"    Tag : " + tag + "    Topic " + Produce.GetTopic()));
			auto result = Produce.SendMsg(packet, tag, mConfig.Mac.c_str());
			if (result.sendStatus == CSendStatus::E_SEND_OK)
			{
				LOG_DAILY(_T("SendMessageSync Sec    group:" + Produce.GetGroup() + "    ip" + Produce.GetIP() +
					"    Tag : " + tag + "    Topic " + Produce.GetTopic()));
				return true;
			}
			else
			{
				LOG(_T("SendMessageSync Error    ip" + Produce.GetIP() +
					"    Tag : " + tag + "    Topic " + Produce.GetTopic() + "Error:" + to_string(result.sendStatus)));
			}
		}
		else
		{
			LOG(_T(std::string("SendMessageSync non find MQ   Tag : ") + tag + "mq:" + to_string(mq)));
		}
#endif
		return false;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s	mq:%d	tag:%s	%s \n", __FUNCTION__, mq, tag, e.what());
		LOG(buffer);
		return false;
	}

}
void UserRoutinue::CachePacket(const MQMessage& rMsg)
{
	std::unique_lock<std::mutex> lock(mCacheLock);
	auto it = mCache.find(rMsg.command);
	if (it != mCache.end())
	{
		it->second = rMsg;
	}
	else
	{
		mCache.insert(std::make_pair(rMsg.command, rMsg));
	}
	WriteCacheFile();
}
void UserRoutinue::DelCachePacket(int command)
{
	std::unique_lock<std::mutex> lock(mCacheLock);
	auto it = mCache.find(command);
	if (it != mCache.end())
	{
		mCache.erase(it);
		WriteCacheFile();
	}
}
void UserRoutinue::ReadCacheFile()
{
	try
	{
		LOG_DAILY("ReadCacheFile Begin");
		mCache.clear();
		ifstream inFile;
#ifdef _WIN32
		inFile.open(PathHelper::GetModuleFolder() + CACHE_FILE, ios::in | ios::binary);
#endif
		if (inFile.is_open())
		{
#ifdef _WIN32
			LOG_DAILY(std::string("Open ") + PathHelper::GetModuleFolder() + CACHE_FILE + " OK");
#endif
			size_t szData = sizeof(MQMessage) - sizeof(cstring);
			size_t strSize = 0;
			while (inFile.read((char *)&strSize, sizeof(strSize)))
			{
				MQMessage iData;
				int readedBytes = inFile.gcount(); //看刚才读了多少字节
				LOG_DAILY(std::string("CACHEFILE  Read size ") + std::to_string(readedBytes));
                if (strSize <= 10000000)
				{
					String_InitSize(&iData.str, strSize, '\0');
				}
				//读字符串
				inFile.read((char*)(iData.str.string), strSize);
				//读其他
				inFile.read((char*)(&(iData)), szData);

				mCache.insert(std::make_pair(iData.command, iData));
				LOG_DAILY(std::string("CACHE  Insert ") + std::to_string(iData.command));
			}
			inFile.close();
		}
		LOG_DAILY("ReadCacheFile End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void UserRoutinue::WriteCacheFile()
{
    ofstream outFile;
#ifdef _WIN32
    outFile.open(PathHelper::GetModuleFolder() + CACHE_FILE, ios::out | ios::binary | ios::trunc);
#endif
	if (outFile.is_open())
	{
		size_t szData = sizeof(MQMessage) - sizeof(cstring);
		for (auto it = mCache.begin(); it != mCache.end(); it++)
		{
			//写字符串长度
			outFile.write((char*)(&(it->second.str.nSize)), sizeof(it->second.str.nSize));
			//写字符串
			outFile.write((char*)(it->second.str.string), it->second.str.nSize);
			//写其他
			outFile.write((char*)(&(it->second)), szData);
		}
		outFile.close();
	}
}
void* UserRoutinue::SendPacketAsync(void* argv)
{
	try
	{
		Msg_Send2MQ* msg = static_cast<Msg_Send2MQ*>(argv);
		if (msg)
		{
			auto pMsg = MSG_NEW_POINT(Msg_Send2MQ, msg);
			SendPacketSync(*pMsg);
			return (void*)1;
		}
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s	%s", __FUNCTION__, e.what());
		LOG(buffer);
        return NULL;
	}
	return 0;
}

#ifdef _WIN32
int ProcessLocalMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt)
{
	std::string utf8;
	char buffer[1024] = { 0 };
	int len = GetMessageBody(msgExt, buffer, sizeof(buffer));
	if (len > sizeof(buffer))
	{
		char* str = (char*)malloc(len);
		len = GetMessageBody(msgExt, str, len);
		utf8.append(str, len);
		free(str);
	}
	else
	{
		utf8.append(buffer, len);
	}
	auto str = Convert::UTF8_To_string(utf8);
	LOG_DAILY(std::string("ProcessMQMessage group: ") + GetPushConsumerGroupID(consumer) + "   data : \n" + str);
	//把消息直接发到User模块
	auto pMsg = MSG_NEW(Msg_RecvMQ);
	pMsg->msg = str;
	pMsg->src = MQ_LOCAL;
	ThreadManager::PushMsg(THREAD_ID::THREAD_USER, pMsg);
	return E_CONSUME_SUCCESS;
}

int ProcessWanMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt)
{
	char buffer[1024] = { 0 };
	int len = GetMessageBody(msgExt, buffer, sizeof(buffer));
	std::string utf8;
	if (len > sizeof(buffer))
	{
		char* str = (char*)malloc(len);
		len = GetMessageBody(msgExt, str, len);
		utf8.append(str, len);
		free(str);
	}
	else
	{
		utf8.append(buffer, len);
	}
	auto str = Convert::UTF8_To_string(utf8);
	LOG_DAILY(std::string("ProcessMQMessage group: ") + GetPushConsumerGroupID(consumer) + "   data : \n" + str);
	//把消息直接发到User模块
	auto pMsg = MSG_NEW(Msg_RecvMQ);
	pMsg->msg = str;
	pMsg->src = MQ_WAN;
	ThreadManager::PushMsg(THREAD_ID::THREAD_USER, pMsg);
	return E_CONSUME_SUCCESS;
}
#endif

