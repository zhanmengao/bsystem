#include<iostream>
#include <string>
using namespace std;

/*
简单工厂：通过专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类

工厂模式：定义一个创建对象的工厂接口，将实际创建工作推迟到子类中
	工厂有香蕉工厂（生产香蕉），苹果工厂（生产苹果）

抽象工厂：提供一个创建一系列相关或者相互依赖的接口，而无需指定它们具体的类
	南方工厂（生产南方香蕉和南方梨）	北方工厂（生产北方香蕉和北方梨）
*/
//水果基类
class Fruit
{
public:
	virtual void shoname() = 0;
};
class Apple :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "苹果哦" << endl;
	}
private:
};
class Pear :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "梨哦" << endl;
	}
private:
};
class EasyFactory
{
public:
	Fruit* Create(char *name)
	{
		if (strcmp(name, "apple") == 0)
		{
			return new Apple;
		}
		else if (strcmp(name, "pear") == 0)
		{
			return new Pear;
		}
	}
};

class Factory
{
public:
	virtual Fruit* Create() = 0;
};
class PearFactory : public Factory
{
	virtual Fruit* Create()
	{
		return new Pear;
	}
};
class appleFactory : public Factory
{
	virtual Fruit* Create()
	{
		return new Apple;
	}
};

class SouthApple :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "南方苹果哦" << endl;
	}
};
class NorthApple :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "北方苹果哦" << endl;
	}
};
class SouthPear :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "南方梨哦" << endl;
	}
};
class NorthPear :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "北方梨哦" << endl;
	}
};
class ABFactory
{
public:
	virtual Fruit* CreatePear() = 0;
	virtual Fruit* CreateApple() = 0;
};
class SouthFactory :public ABFactory
{
public:
	virtual Fruit* CreatePear()
	{
		return new SouthPear;
	}
	virtual Fruit* CreateApple()
	{
		return new SouthApple;
	}
};
class NorthFactory :public ABFactory
{
public:
	virtual Fruit* CreatePear()
	{
		return new NorthPear;
	}
	virtual Fruit* CreateApple()
	{
		return new NorthApple;
	}
};
int main()
{
	EasyFactory *fa1 = new EasyFactory;
	Fruit *fruit = fa1->Create("pear");
	fruit->shoname();
	delete fa1;

	Factory *fa2 = new PearFactory;
	fruit = fa2->Create();
	fruit->shoname();
	fa2 = new appleFactory;
	fruit = fa2->Create();
	fruit->shoname();
	delete fa2;

	ABFactory *fa3 = NULL;
	fa3 = new SouthFactory;
	fruit = fa3->CreateApple();
	fruit->shoname();
	fruit = fa3->CreatePear();
	fruit->shoname();
	delete fa3;
	fa3 = new NorthFactory;
	fruit = fa3->CreatePear();
	fruit->shoname();
	fruit = fa3->CreateApple();
	fruit->shoname();
	delete fa3;
	return 0;
}
