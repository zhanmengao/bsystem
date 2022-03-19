#include<iostream>
#include <string>
using namespace std;
/*
桥接模式：将抽象部分与实际部分分离，使他们都可以独立地变化。

抽象车持有抽象发动机，可以安装任意发动机。
*/
//接口
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
		cout << "安装了" << this->en->sayName() << "引擎"<<endl;
	}
};
class BMW6 :public Car
{
public:
	virtual void installEngine(Engine *en)
	{
		this->en = en;
		cout << "安装了" << this->en->sayName() << "引擎" << endl;
	}
};
class Jeep11 :public Car
{
public:
	virtual void installEngine(Engine *en)
	{
		this->en = en;
		cout << "安装了" << this->en->sayName() << "引擎" << endl;
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