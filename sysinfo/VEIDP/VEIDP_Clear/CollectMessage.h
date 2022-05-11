#pragma once
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadMsg.h"
#include"MSG_ID.h"
#include"stdafx.h"
#include"../../../../Network/Serialization/Json/JsonTools.hpp"
#include<json/value.h>
class Msg_CreateCollect:public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_CREATE_COLLECT;
	}
public:
	std::string orgCode;
};

class Msg_PantCollect :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_PANT_COLLECT;
	}
};

class Msg_CommandCollect :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_COMMAND_COLLECT;
	}
public:
	int command;
	Json::Value rData;
	MQ_TYPE src;
	std::string msg;
};

class Msg_Exit :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_EXIT;
	}
};

class Msg_EndCache :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_END_CACHE;
	}
};

class Msg_EventCollect :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_EVENT_COLLECT;
	}
};
