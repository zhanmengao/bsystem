#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"Telegram.h"
using namespace std;


//消息队列
class TelegramManager
{
private:
	TelegramManager()=delete;
	TelegramManager& operator=(const TelegramManager&) = delete;
	TelegramManager(const TelegramManager&)=delete;
public:
	static bool setMessage(int SenderId, int Received, int MsgType, double DispatchTime=0.0, void *ExtraInfo=nullptr);
	static void DispatchDelayMessage();                    //每次检测是否有消息要发送
private:
	//延迟队列
	static set<Telegram> *DelayPQ;
};