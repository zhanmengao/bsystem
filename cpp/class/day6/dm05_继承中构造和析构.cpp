
#include <iostream>
using namespace std;

//结论
//先 调用父类构造函数 在调用 子类构造函数
//析构的顺序  和构造相反

/*
1、子类对象在创建时会首先调用父类的构造函数
	2、父类构造函数执行结束后，执行子类的构造函数
	3、当父类的构造函数有参数时，需要在子类的初始化列表中显示调用
	4、析构函数调用的先后顺序与构造函数相反
*/

class Parent501
{
public:
	Parent501(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout<<"父类构造函数..."<<endl;
	}
	~Parent501()
	{
		cout<<"析构函数..."<<endl;
	}

	void printP(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout<<"我是爹..."<<endl;
	}
private:
	int a;
	int b;
};


class child501 : public Parent501
{
public:
	child501(int a, int b, int c) : Parent501(a, b)
	{
		this->c = c;
		cout<<"子类的构造函数"<<endl;
	}
	~child501()
	{
		cout<<"子类的析构"<<endl;
	}
	void printC()
	{
		cout<<"我是儿子"<<endl;
	}
protected:
private:
	int c;
};


void playObj()
{
	child501 c1(1, 2, 3);
}
void main501()
{
	//Parent501 p(1, 2);
	playObj();


	cout<<"hello..."<<endl;
	system("pause");
	return ;
}
