
#include <iostream>
using namespace std;

class Parent
{
public:
	Parent(int a)
	{
		this->a = a;
		cout<<"Parent a"<<a<<endl;
	}

	virtual void print() //子类的和父类的函数名字一样
	{
		cout<<"Parent 打印 a:"<<a<<endl;
	}
protected:
private:
	int a ;
};

class Child : public Parent
{
public:
	Child(int b) : Parent(10)
	{
		this->b = b;
		cout<<"Child b"<<b<<endl;
	}
	virtual void print() //virtual 父类写了virtual,子类可写 可不写 
	{
		cout<<"Child 打印  b:"<<b<<endl;
	}
protected:
private:
	int b;
};

void howToPrint(Parent *base)
{
	base->print(); //一种调用语句 有多种表现形态...
}

void howToPrint2(Parent &base)
{
	base.print();
}
void main1301()
{
	
	Parent	*base = NULL;
	Parent	p1(20);
	Child	c1(30);

	base = &p1;
	base->print(); //执行父类的打印函数

	base = &c1;
	base->print(); //执行谁的函数 ?  //面向对象新需求


	{
		Parent &base2 = p1;
		base2.print();

		Parent &base3 = c1; //base3是c1 的别名
		base3.print();
	}


	//函数调用
	howToPrint(&p1);
	howToPrint(&c1);

	howToPrint2(p1);
	howToPrint2(c1);



	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}