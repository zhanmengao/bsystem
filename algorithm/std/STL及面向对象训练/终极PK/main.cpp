#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "Referee.h"
#include "Player.h"
int main(int argc, char* argv[])
{
	//��̨��Ϸ
	Referee referee;  //�����볡
	Player *p1 = new Player("��������");
	Player *p2 = new Player("����");
	referee.entrance(p1,p2);       //�����н��ܶ�λ����
	//��ʼPK
	while (!referee.hasWinner())
	{
		referee.decide();
	}
	//����PK���
	referee.outRefereeInfo();
	delete p1;
	delete p2;
	system("pause");
	return 0;
}