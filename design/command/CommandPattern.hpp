#pragma once
/*
命令模式：将一个请求封装为一个对象，使你可用不同的请求对客户端进行参数化；
	或请求排队或记录请求日志，以及支持可撤销的操作

	护士持有命令，命令医生给病人看病
*/
#include<iostream>
#include<list>
using namespace std;
class Command			//角色1：命令
{
public:
	virtual void Sail() = 0;					//抽象命令：出售
};

class Vendor						//角色2：供应商
{
public:
	void SailBanana()
	{
		cout << "卖香蕉" << endl;
	}
	void SailApple()
	{
		cout << "卖苹果" << endl;
	}
};

//命令子类：出售苹果
class AppleCommand :public Command			//角色1：命令
{
public:
	virtual void Sail()
	{
		mVendor.SailApple();
	}
private:
	Vendor mVendor;
};
//命令子类：出售香蕉
class BananaCommand :public Command			//角色1：命令
{
public:
	virtual void Sail()
	{
		mVendor.SailBanana();
	}
private:
	Vendor mVendor;
};

void mainCommand1()
{
	//通过命令出售水果
	AppleCommand ac;
	ac.Sail();

	BananaCommand bc;
	bc.Sail();
}

class Waiter							//角色3：服务员
{
public:
	void Sail()							//持有命令，可出售商品
	{
		if (pCommand)
		{
			pCommand->Sail();
		}
	}
public:
	Command* pCommand;
};

void mainCommand2()
{
	AppleCommand ac;
	BananaCommand bc;

	//通过服务员调用命令
	Waiter w;
	w.pCommand = &ac;
	w.Sail();

	w.pCommand = &bc;
	w.Sail();
}

class AdvWaiter							//角色3：高级服务员
{
public:
	void Sail()							//全部处理
	{
		for (auto it = pCommandList.begin(); it != pCommandList.end(); it++)
		{
			if (*it)
			{
				(*it)->Sail();
			}
		}
	}
public:
	list<Command*> pCommandList;
};

void mainCommand3()
{
	AppleCommand ac;
	BananaCommand bc;

	//通过高级服务员批量出售
	AdvWaiter aw;
	aw.pCommandList.push_back(&ac);
	aw.pCommandList.push_back(&bc);
	aw.Sail();
}