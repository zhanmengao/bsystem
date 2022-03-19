
#include <iostream>
using namespace std;

class Parent401
{
public:
	void printP()
	{
		cout<<"我是爹..."<<endl;
	}

	Parent401()
	{
		cout<<"parent构造函数"<<endl;
	}

	Parent401(const Parent401 &obj)
	{
		cout<<"copy构造函数"<<endl;
	}

private:
	int a;
};

class child401 : public Parent401
{
public:
	void printC()
	{
		cout<<"我是儿子"<<endl;
	}
protected:
private:
	int c;
};



/*
兼容规则中所指的替代包括以下情况：即：父代表子
	子类对象可以当作父类对象使用
	子类对象可以直接赋值给父类对象
	子类对象可以直接初始化父类对象
	父类指针可以直接指向子类对象
	父类引用可以直接引用子类对象
	*/



//C++编译器 是不会报错的 .....
void howToPrint(Parent401 *base)
{
	base->printP(); //父类的 成员函数 

}

void howToPrint2(Parent401 &base)
{
	base.printP(); //父类的 成员函数 
}

void main401()
{
	//里氏替换原则

	Parent401 p1;
	p1.printP();

	child401 c1;
	c1.printC();
	c1.printP();


	//赋值兼容性原则 
	//1-1 基类指针 (引用) 指向 子类对象
	Parent401 *p = NULL;
	p = &c1;
	p->printP();  

	//1-2 指针做函数参数

	howToPrint(&p1);
	howToPrint(&c1); 

	//1-3引用做函数参数
	howToPrint2(p1);
	howToPrint2(c1); 


	//第二层含义

	//可以让子类对象   初始化   父类对象
	//子类就是一种特殊的父类
	Parent401 p3 = c1;

	
}