#include<iostream>
#include <string>
using namespace std;

/*
�򵥹�����ͨ��ר�Ŷ���һ���������𴴽��������ʵ������������ʵ��ͨ�������й�ͬ�ĸ���

����ģʽ������һ����������Ĺ����ӿڣ���ʵ�ʴ��������Ƴٵ�������
	�������㽶�����������㽶����ƻ������������ƻ����

���󹤳����ṩһ������һϵ����ػ����໥�����Ľӿڣ�������ָ�����Ǿ������
	�Ϸ������������Ϸ��㽶���Ϸ��棩	�������������������㽶�ͱ����棩
*/
//ˮ������
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
		cout << "ƻ��Ŷ" << endl;
	}
private:
};
class Pear :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "��Ŷ" << endl;
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
		cout << "�Ϸ�ƻ��Ŷ" << endl;
	}
};
class NorthApple :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "����ƻ��Ŷ" << endl;
	}
};
class SouthPear :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "�Ϸ���Ŷ" << endl;
	}
};
class NorthPear :public Fruit
{
public:
	virtual void shoname()
	{
		cout << "������Ŷ" << endl;
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
