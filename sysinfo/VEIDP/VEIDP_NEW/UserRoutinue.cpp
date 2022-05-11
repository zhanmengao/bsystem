#include"stdafx.h"
#include"UserRoutinue.h"
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadManager.h"
#include"CollectMessage.h"
#define CACHE_FILE "PacketCache"
void UserRoutinue::Awake()
{
	try
	{
		LOG_DAILY("UserRoutinue Awake Begin");


		msgAIDASet.insert(COMMAND::COMM_COLLALL);
		msgAIDASet.insert(COMMAND::COMM_DISK);
		msgAIDASet.insert(COMMAND::COMM_MEMORY);
		msgAIDASet.insert(COMMAND::COMM_MONIT);
		msgAIDASet.insert(COMMAND::COMM_PRINTER);
		msgAIDASet.insert(COMMAND::COMM_KEY_MOUSE);
		msgAIDASet.insert(COMMAND::COMM_HARDWARE);
		msgAIDASet.insert(COMMAND::COMM_CPU);
		msgAIDASet.insert(COMMAND::COMM_MAINBOARD);

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
	try
	{
		bool bRet = true;
		do
		{
			int iRet = -1;
			LOG_DAILY("UserRoutinue Start Begin");
			mConfig.ReloadConfig();

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
			//开始处理消息
			for (auto it = mMQ.begin(); it != mMQ.end(); it++)
			{
				auto &Consu = it->second.Consu;
				if (Consu.Invalid())
				{
					int ret = Consu.Sub(mConfig.Mac.c_str());
					if (ret == 0)
					{
						LOG_DAILY(_T("StartPushConsumer Sec    group:" + Consu.GetGroup() + "    ip" + Consu.GetIP() +
							"    Topic " + Consu.GetTopic()));
					}
					else
					{
						LOG(_T("StartPushConsumer Fail    group:" + Consu.GetGroup() + "    ip" + Consu.GetIP() +
							"    Topic " + Consu.GetTopic()) + "	error : "+Consu.GetLastError());
						bRet = false;
						break;
					}
				}
				else
				{
					bRet = false;
					break;
				}
			}
			if (!bRet)
			{
				break;
			}
			//从当前目录找到文件
			AIDAPath = PathHelper::GetModuleFolder() + mConfig.aidaPath;
			//手动刷新一次AIDA
			if (mConfig.bStartAida)
			{
				iRet = AidaSUM();
			}
			if (iRet != 0)
			{
				bRet = false;
				break;
			}
			//开机时发送一次采集数据
			auto pMsg = MSG_NEW(Msg_CreateCollect);
			pMsg->orgCode = mConfig.organization;
			ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
		} while (0);
		if (!bRet)
		{
			for (auto it = mMQ.begin(); it != mMQ.end(); it++)
			{
				it->second.Consu.Stop();
			}
			mMQ.clear();
			LOG("UserRoutinue Start Fail");
			return false;
		}
		return true;
		LOG_DAILY("UserRoutinue Start OK");
	}
	catch (exception &e)
	{
		mMQ.clear();
		LOG("UserRoutinue Start fail");
		return false;
	}
}
bool UserRoutinue::Tick()
{
	return false;
}
void UserRoutinue::Destroy()
{
	for (auto it = mMQ.begin(); it != mMQ.end(); it++)
	{
		auto &Consu = it->second.Consu;
		Consu.Stop();
		auto &Produce = it->second.Produce;
		Produce.Stop();
	}
}
int UserRoutinue::AidaSUM()
{
	//将任务扔到线程池调用
	std::unique_lock<std::mutex> lock(mAIDALock);
	if (FileManager::IsFileExist(AIDAPath.c_str()))//判断文件是否有效 TRUE 有效
	{
		LOG_DAILY("调用" + AIDAPath);
		// /R 不弹窗口出来	/ALL 完整报告	/SUM 仅系统摘要	/HW	硬件相关内容 /SW 软件相关内容 /BENCH 性能测试内容 /AUDIT 监视内容 /CUSTOM 自定义选择
		//打开aida采集数据 这个时间有点长，会等待程序采集结束后才会向下执行
		stdstr report = PathHelper::GetModuleFolder() + _T("sum.ini");
		time_t begin = time(NULL);
		if (ForkUtils::ShellExec(AIDAPath, (_T("/SUM /SILENT /INI /R ") + report), true) == FALSE)
		{
			return -1;
		}
		time_t end = time(NULL);
		char buffer[1024] = { 0 };
		sprintf(buffer, "生成成功 ... Path = %s  %ld s \n", report.c_str(), end - begin);
		LOG_DAILY(buffer);
	}
	else
	{
		LOG("调用失败：" + AIDAPath + "	errno = " + to_string(GetLastError()));
		return -1;
	}
	return 0;
}
int UserRoutinue::AidaHW()
{
	std::unique_lock<std::mutex> lock(mAIDALock);
	if (FileManager::IsFileExist(AIDAPath.c_str()))//判断文件是否有效 TRUE 有效
	{
		LOG_DAILY("调用" + AIDAPath);
		// /R 不弹窗口出来	/ALL 完整报告	/SUM 仅系统摘要	/HW	硬件相关内容 /SW 软件相关内容 /BENCH 性能测试内容 /AUDIT 监视内容 /CUSTOM 自定义选择
		//打开aida采集数据 这个时间有点长，会等待程序采集结束后才会向下执行
		stdstr report = PathHelper::GetModuleFolder() + _T("hw.ini");
		time_t begin = time(NULL);
		if(ForkUtils::ShellExec(AIDAPath, (_T("/HW /SILENT /INI /R ") + report), true) == FALSE)
		{
			return -1;
		}
		time_t end = time(NULL);
		char buffer[1024] = { 0 };
		sprintf(buffer, "生成成功 ... Path = %s  %ld s\n", report.c_str(), end - begin);
		LOG_DAILY(buffer);
	}
	else
	{
		LOG("调用失败：" + AIDAPath + "	errno = " + to_string(GetLastError()));
		return -1;
	}
	return 0;
}
bool UserRoutinue::IsMsgAIDA(COMMAND c)
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
		if (rMsg.tag >= TAG_MAX || rMsg.tag < 0)
		{
			char buffer[1024] = { 0 };
			sprintf(buffer, "%s tag(%d) >= TAG_MAX(%d)", __FUNCTION__, rMsg.tag, TAG_MAX);
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
		inFile.open(PathHelper::GetModuleFolder() + CACHE_FILE, ios::in | ios::binary);
		if (inFile.is_open())
		{
			LOG_DAILY(std::string("Open ") + PathHelper::GetModuleFolder() + CACHE_FILE + " OK");
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
				if (iData.str.string != NULL)
				{
					inFile.read((char*)(iData.str.string), strSize);
					//读其他
					inFile.read((char*)(&(iData)), szData);

					mCache.insert(std::make_pair(iData.command, iData));
				}
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
	ofstream outFile(PathHelper::GetModuleFolder() + CACHE_FILE, ios::out | ios::binary | ios::trunc);
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
void* UserRoutinue::OnCommandCallAIDA(void* argv)
{
	AidaHW();
	Msg_CommandCollect* nMsg = static_cast<Msg_CommandCollect*>(argv);
	if (nMsg)
	{
		//解析完毕，将消息发给收集线程
		auto pMsg = MSG_NEW_POINT(Msg_CommandCollect, nMsg);
		ThreadManager::PushMsg(THREAD_ID::THREAD_COLLECT, pMsg);
		return (void*)1;
	}
	return 0;
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
		return false;
	}
	return 0;
}

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

