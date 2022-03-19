#include<iostream>
#include<string>
#include<vector>
using namespace std;


//消息体
class Telegram{
public:
	Telegram(int SenderId, int Received, int MsgType, double DispatchTime, void* ExtraInfo=nullptr)
		:SenderId(SenderId), Received(Received), MsgType(MsgType), DispatchTime(DispatchTime), ExtraInfo(ExtraInfo)
	{

	}
public:
	int getSendId()const
	{
		return SenderId;
	}
	void setSendId(const int& SenderId)
	{
		this->SenderId = SenderId;
	}
	int getReceived() const
	{
		return this->Received;
	}
	void setReceived(const int& Received)
	{
		this->Received = Received;
	}
	int getMsgType() const
	{
		return MsgType;
	}
	void setMsgType(double MsgType)
	{
		MsgType = MsgType;
	}

protected:
private:
	int SenderId;             //发送方ID
	int Received;             //接收方ID
	int MsgType;              //消息码
	double DispatchTime;       //这条消息隔多久后发送
	void * ExtraInfo;          //消息体
};