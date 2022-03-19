#include<iostream>
#include <string>
using namespace std;
/*
�Ž�ģʽ�������󲿷���ʵ�ʲ��ַ��룬ʹ���Ƕ����Զ����ر仯��

���󳵳��г��󷢶��������԰�װ���ⷢ������
*/
//�ӿ�
class Engine
{
public:
	virtual string sayName() = 0;
};
class Engine400 :public Engine{
public:
	virtual string sayName()
	{
		return "Engine400";
	}
};
class Engine600 :public Engine
{
public:
	virtual string sayName()
	{
		return "Engine600";
	}
};
class Engine1100CC :public Engine
{
public:
	virtual string sayName()
	{
		return "Engine1100CC";
	}
};

class Car
{
public:
	virtual void installEngine(Engine *en) = 0;
protected:
	Engine *en;
};
class BMW :public Car
{
public:
	virtual void installEngine(Engine *en)
	{
		this->en = en;
		cout << "��װ��" << this->en->sayName() << "����"<<endl;
	}
};
class BMW6 :public Car
{
public:
	virtual void installEngine(Engine *en)
	{
		this->en = en;
		cout << "��װ��" << this->en->sayName() << "����" << endl;
	}
};
class Jeep11 :public Car
{
public:
	virtual void installEngine(Engine *en)
	{
		this->en = en;
		cout << "��װ��" << this->en->sayName() << "����" << endl;
	}
};



void BuidgePattern()
{
	Car *car = new BMW();
	Engine *en = new Engine400;
	car->installEngine(en);

	en = new Engine600;
	car = new Jeep11;
	car->installEngine(en);
	delete en;
	delete car;
}