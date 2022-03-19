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
//攻击对方

void Player::Attack(IPlayer *enemy)
{
	Fight *f = this->fight();  //产生招式
	cout << "[" + _name + "]使出" + f->getName() + ",";
	enemy->onAttack(f);   //揍他！
}
//学习武艺
void Player::study()
{
	this->_fightList.push_back(Fight("降龙十八掌",Damage(30,Damage::Blood, false,0) ));
	this->_fightList.push_back(Fight("葵花宝典", Damage(40, Damage::Blood, false, 0)));
	this->_fightList.push_back(Fight("化功大法", Damage(0.4, Damage::Persont, false, 0)));
	this->_fightList.push_back(Fight("寒冰掌", Damage(10, Damage::Blood, true, 3)));

	this->_defense.push_back(Defense("凌波微步", Damage(0.7, Damage::Persont, false, 0)));
	this->_defense.push_back(Defense("金钟罩", Damage(0.5, Damage::Persont, false, 0)));
	this->_defense.push_back(Defense("格挡", Damage(0.2, Damage::Persont, false, 0)));
}
//受到攻击动作
void Player::onAttack(Fight* fightData)
{
	//产生防守动作
	Defense* defense = this->defense(); 
	//处理受到的本次伤害
	this->computDamage(fightData, defense);
	cout << "[" + _name + "]" + defense->getName() + ",";
	//处理持续伤害
	if (this->_sustainedNum--)
	{
		this->computDamage(&(this->_sustained), defense);
		cout << "[" + _name + "]受到" + _sustained.getName() + "持续伤害,";
	}
	//判断本次攻击动作是否可持续
	if (fightData->getDamage().isSustained())
	{
		this->_sustained = *fightData;
		this->_sustainedNum = this->_sustained.getDamage().sustainedRound() - 1;
	}
}

void Player::computDamage(Fight* fightData, Defense* defenseData)
{
	//Defense *defense = this->defense();
	//计算本次掉血量
	double downBlood = fightData->getDamage().getValue() * (1.0 - defenseData->getDamage().getValue());//注意这一句的防御规则被写死了，可以根据新需求修改代码
	if (fightData->getDamage().getMode() == Damage::Blood)  //血量作用
	{
		this->_blood -= (int)downBlood;
	}
	else if (fightData->getDamage().getMode() == Damage::Persont) //百分比作用
	{
		this->_blood *= 1.0 - downBlood;
	}
}
//随机招式数据
Fight* Player::fight()
{
	int idx = rand() % this->_fightList.size();
	return &(this->_fightList[idx]);
}
//随机防御数据
Defense* Player::defense()
{
	int idx = rand() % this->_defense.size();
	return &(this->_defense[idx]);
}