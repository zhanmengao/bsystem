#pragma once
#include "Player.h"
//����
class Referee
{
public:
	Referee();
	~Referee();
	void entrance(Player *p1, Player *p2);  //�볡
	void decide();   //�ٲ�һ���غ�
	bool hasWinner(); //�ٲ��Ƿ����ʤ����
	void outRefereeInfo();//��������ٲ���Ϣ
private:
	Player* players[2];
};

