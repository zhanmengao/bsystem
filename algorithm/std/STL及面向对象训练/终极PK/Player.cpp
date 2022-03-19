#include "Player.h"
#include <math.h>
#include <time.h>


Player::Player(string name) :_name(name), _blood(100), _sustainedNum(0)
{
	srand(time(NULL));
	this->study();
}
Player::~Player()
{

}

string Player::getName()
{
	return this->_name;
}
//�����Է�

void Player::Attack(IPlayer *enemy)
{
	Fight *f = this->fight();  //������ʽ
	cout << "[" + _name + "]ʹ��" + f->getName() + ",";
	enemy->onAttack(f);   //������
}
//ѧϰ����
void Player::study()
{
	this->_fightList.push_back(Fight("����ʮ����",Damage(30,Damage::Blood, false,0) ));
	this->_fightList.push_back(Fight("��������", Damage(40, Damage::Blood, false, 0)));
	this->_fightList.push_back(Fight("������", Damage(0.4, Damage::Persont, false, 0)));
	this->_fightList.push_back(Fight("������", Damage(10, Damage::Blood, true, 3)));

	this->_defense.push_back(Defense("�貨΢��", Damage(0.7, Damage::Persont, false, 0)));
	this->_defense.push_back(Defense("������", Damage(0.5, Damage::Persont, false, 0)));
	this->_defense.push_back(Defense("��", Damage(0.2, Damage::Persont, false, 0)));
}
//�ܵ���������
void Player::onAttack(Fight* fightData)
{
	//�������ض���
	Defense* defense = this->defense(); 
	//�����ܵ��ı����˺�
	this->computDamage(fightData, defense);
	cout << "[" + _name + "]" + defense->getName() + ",";
	//��������˺�
	if (this->_sustainedNum--)
	{
		this->computDamage(&(this->_sustained), defense);
		cout << "[" + _name + "]�ܵ�" + _sustained.getName() + "�����˺�,";
	}
	//�жϱ��ι��������Ƿ�ɳ���
	if (fightData->getDamage().isSustained())
	{
		this->_sustained = *fightData;
		this->_sustainedNum = this->_sustained.getDamage().sustainedRound() - 1;
	}
}

void Player::computDamage(Fight* fightData, Defense* defenseData)
{
	//Defense *defense = this->defense();
	//���㱾�ε�Ѫ��
	double downBlood = fightData->getDamage().getValue() * (1.0 - defenseData->getDamage().getValue());//ע����һ��ķ�������д���ˣ����Ը����������޸Ĵ���
	if (fightData->getDamage().getMode() == Damage::Blood)  //Ѫ������
	{
		this->_blood -= (int)downBlood;
	}
	else if (fightData->getDamage().getMode() == Damage::Persont) //�ٷֱ�����
	{
		this->_blood *= 1.0 - downBlood;
	}
}
//�����ʽ����
Fight* Player::fight()
{
	int idx = rand() % this->_fightList.size();
	return &(this->_fightList[idx]);
}
//�����������
Defense* Player::defense()
{
	int idx = rand() % this->_defense.size();
	return &(this->_defense[idx]);
}