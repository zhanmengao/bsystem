#include<iostream>
using namespace std;

class Parent1{
public:
	void printP(int a,int b)
	{
		this->a = a;
		this->b = b;
		cout << "���ǰְ�" << endl;
	}
	Parent1(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout << "���๹�캯��" << endl;
	}
	Parent1(const Parent1& obj)
	{
		cout << "���࿽������" << endl;
	}
	~Parent1()
	{
		cout << "������������" << endl;
	}
private:
	int a,b;
};

class Child1 : public Parent1{
public:
	void printC()
	{
		cout << "������" << endl;
	}
	Child1(int a, int b, int c) :Parent1(a, b)
	{
		this->c = c;
		cout << "���๹�캯��" << endl;
	};
	~Child1()
	{
		cout << "������������" << endl;
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
