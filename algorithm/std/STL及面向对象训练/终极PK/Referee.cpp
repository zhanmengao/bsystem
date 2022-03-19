#include "Referee.h"


Referee::Referee()
{
	this->players[0] = NULL;
	this->players[1] = NULL;
	//this->
}

Referee::~Referee()
{
}
//�볡
void Referee::entrance(Player *p1, Player *p2)
{
	this->players[0] = p1;
	this->players[1] = p2;
}
//�ٲ�һ���غ�
void Referee::decide()
{
	//ע�⣬��Ҫ��ʼװB�ˣ�
	static int attackerId = 0;
	this->players[attackerId]->Attack(this->players[1 - attackerId]);
	attackerId = 1 - attackerId;
	cout << "(" +  players[0]->getName() + "Ѫ����" << players[0]->getBlood() << ",";
	cout << players[1]->getName() + "Ѫ����" << players[1]->getBlood() << ")" << endl;
}

//�ٲ��Ƿ����ʤ����
bool Referee::hasWinner()
{
	return this->players[0]->getBlood() <= 0 || this->players[1]->getBlood() <= 0;
}

//��������ٲ���Ϣ
void Referee::outRefereeInfo()
{
	int loserId = this->players[0]->getBlood() < this->players[1]->getBlood() ? 0 : 1;
	cout << this->players[loserId]->getName() << "��" << this->players[1 - loserId]->getName() << "�����ˡ�����" << endl;
}
