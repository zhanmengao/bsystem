#include<iostream>
using namespace std;

class Parent1{
public:
	void printP(int a,int b)
	{
		this->a = a;
		this->b = b;
		cout << "我是爸爸" << endl;
	}
	Parent1(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout << "父类构造函数" << endl;
	}
	Parent1(const Parent1& obj)
	{
		cout << "父类拷贝构造" << endl;
	}
	~Parent1()
	{
		cout << "父类析构函数" << endl;
	}
private:
	int a,b;
};

class Child1 : public Parent1{
public:
	void printC()
	{
		cout << "我是崽" << endl;
	}
	Child1(int a, int b, int c) :Parent1(a, b)
	{
		this->c = c;
		cout << "子类构造函数" << endl;
	};
	~Child1()
	{
		cout << "子类析构函数" << endl;
	}
private:
	int c;
};

void haotoPrint(Parent1 *base)
{
	base->printP(1,2);
}


void mainplayer()
{
	Child1(1, 2, 3);

	
}
int main()
{
	mainplayer();
	return 0;
}
