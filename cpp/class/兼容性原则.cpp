#include<iostream>
using namespace std;

class Parent2{
public:
	void printP()
	{
		cout << "我是爸爸" << endl;
	}
	Parent2() = default;
	Parent2(const Parent2& obj)
	{
		cout << "父类拷贝构造" << endl;
	}
private:
	int a;
};

class Child2 : public Parent2{
public:
	void printC()
	{
		cout << "我是崽" << endl;
	}
	Child2() = default;

private:
	int c;
};

void haotoPrint(Parent2 *base)
{
	base->printP();
}


void main2()
{	
	Child2 c1;
	Parent2 p1 = c1;
	//原则：
	//1、基类指针可以指向子类对象
	Parent2 *p = NULL;
	p = &c1;
	p->printP();
	haotoPrint(&c1);

	system("pause");
}
