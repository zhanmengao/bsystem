#pragma once
#include "State.h"
#include "Miner.h"


//�ڿ�״̬�������л�����ˮ����Ǯ��WC
class EnterMineAndDigForNugget :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static EnterMineAndDigForNugget* Instance();
};

//ȥ���д�Ǯ�������л���WC���ڿ󡢻ؼ�
class VisitBankAndDepositGold :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static VisitBankAndDepositGold* Instance();
};

//�ؼ���Ϣ�������л����ڿ�WC
class GoHomeAndSleepTillRested :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static GoHomeAndSleepTillRested* Instance();
};

//�ڿ�ȥ�ư���� �����л����ڿ�
class QuenchThirst :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static QuenchThirst* Instance();
};

//�ϲ������л���ԭ��״̬
class MinerGlobalState :public State < Miner >
{
public:
	virtual void Enter(Miner *pOnwer);
	virtual void Execute(Miner *pOnwer);
	virtual void Exit(Miner *pOnwer);
	static MinerGlobalState* Instance();
};