#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include "Miner.h"
using namespace std;

void Miner::Update()
{
	if (m_pMachine != nullptr)
	{
		this->m_pMachine->Update();
	}
}

void Miner::ChangeLocation(const position& newLocation)
{
	this->m_Location=newLocation;
	cout << "Bob移动到"<<endl;
}

void Miner::AddToGoldCarried(const unsigned& gold)
{
	this->m_iGoldCarried += gold;
}

void Miner::SaveToBank()
{
	this->m_iMoneyInBank += this->m_iGoldCarried;
	this->m_iGoldCarried = 0;
	cout << "Bob将钱全部存到银行"<<endl;
}

bool Miner::isPocketsFull()
{
	return this->m_iGoldCarried >= 100;
}

void Miner::Sleep()
{
	this->m_iThirst--;
	cout << "Bob在家睡觉"<<endl;
}

bool Miner::isSleepy()
{
	return this->m_iFatigue >= 100;
}

void Miner::Drink()
{
	this->m_iFatigue--;
	cout << "Bob在酒吧喝牛奶"<<endl;
}

bool Miner::isThirsty()
{
	return this->m_iThirst >= 100;
}

bool Miner::isWanttoWC()
{

}

StateMachine<Miner>& Miner::getMachine()
{
	return *(this->m_pMachine);
}

