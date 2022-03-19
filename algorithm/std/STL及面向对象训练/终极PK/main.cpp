#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "Referee.h"
#include "Player.h"
int main(int argc, char* argv[])
{
	//搭台唱戏
	Referee referee;  //裁判入场
	Player *p1 = new Player("东方不败");
	Player *p2 = new Player("萧峰");
	referee.entrance(p1,p2);       //给裁判介绍二位大侠
	//开始PK
	while (!referee.hasWinner())
	{
		referee.decide();
	}
	//宣布PK结果
	referee.outRefereeInfo();
	delete p1;
	delete p2;
	system("pause");
	return 0;
}