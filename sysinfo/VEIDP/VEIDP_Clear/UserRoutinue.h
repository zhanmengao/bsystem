#pragma once
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadBase.h"
#include"THREAD_ID.h"
#include"Config.h"
#include"UserMessage.h"
#ifdef _WIN32
#include"../../../../Network/MQ/RocketMQ/RocketMQ/RockMQProduUtils.h"
#include"../../../../Network/MQ/RocketMQ/PushConsumer/RockMQConsuUtils.h"
#endif
#include<set>
#include<map>
extern "C"
{
#include"../../../../algorithm/Normal/byd/stb/CString.h"
}

#ifdef _WIN32
struct RockMQ
{
	RockMQProdu Produce;
	RockMQConsu Consu;
};
#endif
struct MQMessage
{
	int tag;
	int target = MQ_LOCAL;
	int command;
	cstring str = { 0 };
	MQMessage()
	{
		String_InitSize(&str, 0, '\0');
	}
	~MQMessage()
	{
		String_Free(&str);
	}
	MQMessage(const MQMessage& obj):target(obj.target), command(obj.command),tag(obj.tag)
	{
		String_CopyFrom(&str, &obj.str);
	}
    MQMessage& operator=(const MQMessage& obj)
	{
		if (*this == obj)
		{
			return *this;
		}
		else
		{
			tag = obj.tag;
			String_CopyFrom(&str, &obj.str);
			target = obj.target;
			command = obj.command;
			return *this;
		}
	}
	MQMessage(const Msg_Send2MQ& obj) :target(obj.target), command(obj.command),tag(obj.tag)
	{
		String_InitStr(&str, obj.rData.c_str(),obj.rData.size());
	}
    MQMessage& operator=(const Msg_Send2MQ& obj)
	{
		if (*this == obj)
		{
			return *this;
		}
		else
		{
			tag = obj.tag;
			String_InitStr(&str, obj.rData.c_str(), obj.rData.size());
			target = obj.target;
			command = obj.command;
			return *this;
		}
	}
    bool operator==(const MQMessage& obj)
	{
		if ((strcmp(str.string , obj.str.string) == 0) && (tag == obj.tag))
		{
			return true;
		}
		return false;
	}
    bool operator==(const Msg_Send2MQ& obj)
	{
		if ((obj.rData == str.string) && (tag == obj.tag))
		{
			return true;
		}
		return false;
	}
};
class UserRoutinue :public ThreadBase
{
public:
	virtual unsigned int GetId() const
	{
		return THREAD_USER;
	}
	virtual unsigned int TickTime() const
	{
		return 500;
	}
	virtual DESTROY_SORT DestroySort() const
	{
		return DESTROY_SORT::DESTROY_NORMAL;
	}
public:
	UserRoutinue()
	{
		Awake();
	}
protected:
	virtual void ProcessMsg(MsgBase &rMsg);
	virtual void Awake();
	virtual bool Start();
	virtual bool Tick();
	virtual void Destroy();
private:
	void SendPacketSync(const Msg_Send2MQ& rMsg);
	bool SendPacketSync(const std::string& packet,const char* tag, int mq);
	void CachePacket(const MQMessage& rMsg);
	void DelCachePacket(int command);
	void ReadCacheFile();
	void WriteCacheFile();
	void HandleMsg(const Msg_Create& rMsg);
	void HandleMsg(const Msg_Send2MQ& rMsg);
	void HandleMsg(const Msg_RecvMQ& rMsg);
	void HandleMsg(const Msg_SetVetsion& rMsg);
	void HandleMsg(const Msg_SetOrgcode& rMsg);
private:
	//async
	void* SendPacketAsync(void* argv);
private:
	Config mConfig;
#ifdef _WIN32
	std::map<int, RockMQ> mMQ;
#endif
	std::map<int, MQMessage> mCache;
	std::mutex mCacheLock;
	const char* mTags[TAG_MAX];
};

#ifdef _WIN32
int ProcessLocalMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt);
int ProcessWanMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt);
void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void CALLBACK Tick_Pant(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
void OnWMCreate(HWND hWnd);
void OnWMDevice(HWND hWnd, WPARAM wParam, LPARAM lParam);
void OnWMEnd(HWND hWnd);
#endif
/*
Config
USB
MQ
AIDAË¢ÐÂ
*/
