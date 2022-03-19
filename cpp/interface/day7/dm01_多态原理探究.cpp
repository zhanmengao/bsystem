
#include <iostream>
using namespace std;

//多态成立的三个条件 
//要有继承  虚函数重写  父类指针指向子类对象 

class Parent101
{
public:
	Parent101(int a = 0)
	{
		this->a = a;
	}

	virtual void print()  //1 动手脚  写virtal关键字 会特殊处理 //虚函数表
	{
		cout<<"我是爹"<<endl;
	}
	virtual void print2()  //1 动手脚  写virtal关键字 会特殊处理 //虚函数表
	{
		cout<<"我是爹"<<endl;
	}
private:
	int a;
};

class Child101 : public Parent101
{
public:
	Child101(int a = 0, int b = 0) :Parent101(a)
	{
		this->b = b;
	}

	virtual void print()
	{
		cout<<"我是儿子"<<endl;
	}
private:
	int b;
};

void HowToPlay1(Parent101 *base)
{
	base->print(); //有多态发生  //2 动手脚  
	//效果:传来子类对 执行子类的print函数 传来父类对执行父类的print函数 
	//C++编译器根本不需要区分是子类对象 还是父类对象
	//父类对象和子类对象分步有vptr指针 , ==>虚函数表===>函数的入口地址
	//迟绑定 (运行时的时候,c++编译器才去判断)
}

void main101()
{

	Parent101	p1; //3 动手脚 提前布局  
				//用类定义对象的时候 C++编译器会在对象中添加一个vptr指针 
	Child101	c1; //子类里面也有一个vptr指针

	HowToPlay1(&p1);
	HowToPlay1(&c1);
}