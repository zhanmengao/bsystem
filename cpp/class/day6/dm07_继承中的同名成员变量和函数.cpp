
#include <iostream>
using namespace std;


class A701
{
public:
	int a;
	int b;
public:
	void get()
	{
		cout<<"b "<<b<<endl;
	}
	void print()
	{
		cout<<"AAAAA "<<endl;
	}
protected:
private:
};

class B701 : public A701
{
public:
	int b;//������A�е�ͬ������
	int c;
public:
	void get_child()
	{
		cout<<"b "<<b<<endl;
	}
	void print()
	{
		cout<<"BBBB "<<endl;
	}
protected:
private:
};

void main701()
{
	B701 b1;
	b1.print(); 

	b1.A701::print();
	b1.B701::print(); //Ĭ�����

	system("pause");
}

//ͬ����Ա����
void main702()
{
	
	B701 b1;
	b1.b = 1; 
	b1.get_child();

	b1.A701::b = 100; //�޸ĸ����b
	b1.B701::b = 200; //�޸������b Ĭ�������B

	b1.get();


	cout<<"hello..."<<endl;
	system("pause");
	return ;
}