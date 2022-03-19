
#include <iostream>
using namespace std;

//构造函数中调用虚函数能发生多态吗?

class Parent301
{
public:
	Parent301(int a = 0)
	{
		this->a = a;
		print();        //调用：我是爹 因为此时虚表指针还未初始化
	}

	virtual void print()  
	{
		cout<<"我是爹"<<endl;
	}
	
private:
	int a;
};

class Child301 : public Parent301
{
public:
	Child301(int a = 0, int b = 0) :Parent301(a)
	{
		this->b = b;
		print();
	}

	virtual void print()
	{
		cout<<"我是儿子"<<endl;
	}
private:
	int b;
};

void main301()
{
	Child301  c1; //定义一个子类对象 ,在这个过程中,在父类构造函数中调用虚函数print 能发生多态吗?
	
	//c1.print();
}