#pragma once
#include "kongfu.h"
#include <string>
#include <vector>
using namespace std;
class IPlayer
{
public:
	virtual string getName() = 0;
	virtual void Attack(IPlayer *enemy)=0;		//�����Է�
	virtual void onAttack(Fight* fightData) = 0;   //�ܵ���������
	virtual int getBlood() = 0;   //�õ���ǰѪ��
};

class Player :public IPlayer
{
public:
	Player(string name);
	~Player();
	virtual string getName();
	virtual void Attack(IPlayer *enemy);		//�����Է�
	virtual void onAttack(Fight* fightData);	//�ܵ���������
	virtual int getBlood(){ return this->_blood; }   //�õ���ǰѪ��
	virtual void study();	//ѧϰ����
private:
	Fight* fight();  //�õ�ս����ʽ
	Defense* defense();   //�õ�������ʽ
	void computDamage(Fight* fightData, Defense* defenseData);	//������ʧ

	vector<Fight> _fightList;  //ѧ��Ľ�����ʽ
	vector<Defense> _defense;  //ѧ��ķ�����ʽ

	Fight _sustained;	//���������˺�Ч��
	int _sustainedNum;			//ʣ������˺�����
	string _name;
	int _blood;
};

