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
//入场
void Referee::entrance(Player *p1, Player *p2)
{
	this->players[0] = p1;
	this->players[1] = p2;
}
//仲裁一个回合
void Referee::decide()
{
	//注意，我要开始装B了！
	static int attackerId = 0;
	this->players[attackerId]->Attack(this->players[1 - attackerId]);
	attackerId = 1 - attackerId;
	cout << "(" +  players[0]->getName() + "血量：" << players[0]->getBlood() << ",";
	cout << players[1]->getName() + "血量：" << players[1]->getBlood() << ")" << endl;
}

//仲裁是否存在胜出者
bool Referee::hasWinner()
{
	return this->players[0]->getBlood() <= 0 || this->players[1]->getBlood() <= 0;
}

//输出最终仲裁信息
void Referee::outRefereeInfo()
{
	int loserId = this->players[0]->getBlood() < this->players[1]->getBlood() ? 0 : 1;
	cout << this->players[loserId]->getName() << "被" << this->players[1 - loserId]->getName() << "玩死了。。。" << endl;
}
