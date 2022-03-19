#include "TelegramManager.h"

set<Telegram> *TelegramManager::DelayPQ=new set<Telegram>;


bool TelegramManager::setMessage(int SenderId, int Received, int MsgType, double DispatchTime, void *ExtraInfo)
{

}
void TelegramManager::DispatchDelayMessage()                   //每次检测是否有消息要发送
{
	for (auto it = DelayPQ->begin(); it != DelayPQ->end();)
	{
		//TODO 处理消息

		//发送后删除消息 TODO
	}
}
