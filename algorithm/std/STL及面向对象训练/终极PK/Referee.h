#pragma once
#include "Player.h"
//裁判
class Referee
{
public:
	Referee();
	~Referee();
	void entrance(Player *p1, Player *p2);  //入场
	void decide();   //仲裁一个回合
	bool hasWinner(); //仲裁是否存在胜出者
	void outRefereeInfo();//输出最终仲裁信息
private:
	Player* players[2];
};

