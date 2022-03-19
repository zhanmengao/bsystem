#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"Telegram.h"
using namespace std;


//��Ϣ����
class TelegramManager
{
private:
	TelegramManager()=delete;
	TelegramManager& operator=(const TelegramManager&) = delete;
	TelegramManager(const TelegramManager&)=delete;
public:
	static bool setMessage(int SenderId, int Received, int MsgType, double DispatchTime=0.0, void *ExtraInfo=nullptr);
	static void DispatchDelayMessage();                    //ÿ�μ���Ƿ�����ϢҪ����
private:
	//�ӳٶ���
	static set<Telegram> *DelayPQ;
};