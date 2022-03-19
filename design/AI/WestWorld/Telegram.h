#include<iostream>
#include<string>
#include<vector>
using namespace std;


//��Ϣ��
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
	int SenderId;             //���ͷ�ID
	int Received;             //���շ�ID
	int MsgType;              //��Ϣ��
	double DispatchTime;       //������Ϣ����ú���
	void * ExtraInfo;          //��Ϣ��
};