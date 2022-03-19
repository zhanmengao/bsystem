#pragma once
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadMsg.h"
#include"MSG_ID.h"
#include"stdafx.h"
#include"../../../../Network/Serialization/Json/JsonTools.hpp"
class Msg_Create :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_CREATE;
	}
public:
	HWND hWnd;
};

class Msg_Send2MQ :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_SEND_MQ;
	}
public:
	std::string rData;
	int tag;
	MQ_TYPE target  = MQ_LOCAL;
	int command;
};

class Msg_RecvMQ :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_RECV_MQ;
	}
public:
	std::string msg;
	MQ_TYPE src;
};

class Msg_SetVetsion :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_SET_VERSION;
	}
public:
	std::string version;
	std::string url;
	std::string md5;
	int updateType;
};

class Msg_SetOrgcode :public MsgBase
{
public:
	virtual unsigned int GetId() const
	{
		return MSG_ID::MSG_SET_ORGCODE;
	}
public:
	std::string orgcode;
};