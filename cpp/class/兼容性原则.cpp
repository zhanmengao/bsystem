#include<iostream>
using namespace std;

class Parent2{
public:
	void printP()
	{
		cout << "���ǰְ�" << endl;
	}
	Parent2() = default;
	Parent2(const Parent2& obj)
	{
		cout << "���࿽������" << endl;
	}
private:
	int a;
};

class Child2 : public Parent2{
public:
	void printC()
	{
		cout << "������" << endl;
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
	//ԭ��
	//1������ָ�����ָ���������
	Parent2 *p = NULL;
	p = &c1;
	p->printP();
	haotoPrint(&c1);

	system("pause");
}
