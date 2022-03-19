#pragma once
/*
����ģʽ����һ�������װΪһ������ʹ����ò�ͬ������Կͻ��˽��в�������
	�������Ŷӻ��¼������־���Լ�֧�ֿɳ����Ĳ���

	��ʿ�����������ҽ�������˿���
*/
#include<iostream>
#include<list>
using namespace std;
class Command			//��ɫ1������
{
public:
	virtual void Sail() = 0;					//�����������
};

class Vendor						//��ɫ2����Ӧ��
{
public:
	void SailBanana()
	{
		cout << "���㽶" << endl;
	}
	void SailApple()
	{
		cout << "��ƻ��" << endl;
	}
};

//�������ࣺ����ƻ��
class AppleCommand :public Command			//��ɫ1������
{
public:
	virtual void Sail()
	{
		mVendor.SailApple();
	}
private:
	Vendor mVendor;
};
//�������ࣺ�����㽶
class BananaCommand :public Command			//��ɫ1������
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
	//ͨ���������ˮ��
	AppleCommand ac;
	ac.Sail();

	BananaCommand bc;
	bc.Sail();
}

class Waiter							//��ɫ3������Ա
{
public:
	void Sail()							//��������ɳ�����Ʒ
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

	//ͨ������Ա��������
	Waiter w;
	w.pCommand = &ac;
	w.Sail();

	w.pCommand = &bc;
	w.Sail();
}

class AdvWaiter							//��ɫ3���߼�����Ա
{
public:
	void Sail()							//ȫ������
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

	//ͨ���߼�����Ա��������
	AdvWaiter aw;
	aw.pCommandList.push_back(&ac);
	aw.pCommandList.push_back(&bc);
	aw.Sail();
}