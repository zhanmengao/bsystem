#pragma once
#include "State.h"
#include "Miner.h"


//挖矿状态：可以切换到喝水、存钱、WC
class EnterMineAndDigForNugget :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static EnterMineAndDigForNugget* Instance();
};

//去银行存钱，可以切换到WC、挖矿、回家
class VisitBankAndDepositGold :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static VisitBankAndDepositGold* Instance();
};

//回家休息，可以切换到挖矿、WC
class GoHomeAndSleepTillRested :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static GoHomeAndSleepTillRested* Instance();
};

//口渴去酒吧买酒 可以切换到挖矿
class QuenchThirst :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static QuenchThirst* Instance();
};

//上厕所，切换回原先状态
class MinerGlobalState :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static MinerGlobalState* Instance();
};