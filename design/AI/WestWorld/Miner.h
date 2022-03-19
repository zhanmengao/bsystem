#include<iostream>
#include<string>
#include<vector>
#include "StateMachine.h"
#include "BaseGameEntity.h"
#include "Position.hpp"
using namespace std;


const position& MinePos = { 1, 1, 1 };
const position& BankPos = { 2, 2, 2 };
const position& HomePos = { 3, 3, 3, };
const position& BarPos = { 4, 4, 4, };
const position& WCPos = { 5, 5, 5, };



//旷工类
class Miner :public BaseGameEntity
{
public:
	Miner(int id) :BaseGameEntity(id), m_iGoldCarried(0), m_iMoneyInBank(0), m_iThirst(0), m_iFatigue(0)
	{
		m_pMachine = new StateMachine<Miner>(this);
		m_pMachine->setCurrentState(EnterMineAndDigForNugget::Instance());
		m_pMachine->setGlovalState(MinerGlobalState::Instance());	
	}
	~Miner(){
		if (m_pMachine != nullptr)
		{
			delete m_pMachine;
			m_pMachine = nullptr;
		}	
	}
public:
	void Update();
	void ChangeLocation(const position& newLocation);
	void AddToGoldCarried(const unsigned& gold);
	bool isPocketsFull();
	void SaveToBank();
	void Sleep();
	bool isSleepy();
	void Drink();
	bool isThirsty();
	bool isWanttoWC();
	StateMachine<Miner>& getMachine();
private:
	position m_Location;
	unsigned m_iGoldCarried;
	unsigned m_iMoneyInBank;
	unsigned m_iThirst;
	unsigned m_iFatigue;
	//指向状态机的指针
	StateMachine<Miner> *m_pMachine;
};

