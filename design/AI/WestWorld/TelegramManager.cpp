#include "TelegramManager.h"

set<Telegram> *TelegramManager::DelayPQ=new set<Telegram>;


bool TelegramManager::setMessage(int SenderId, int Received, int MsgType, double DispatchTime, void *ExtraInfo)
{

}
void TelegramManager::DispatchDelayMessage()                   //ÿ�μ���Ƿ�����ϢҪ����
{
	for (auto it = DelayPQ->begin(); it != DelayPQ->end();)
	{
		//TODO ������Ϣ

		//���ͺ�ɾ����Ϣ TODO
	}
}
