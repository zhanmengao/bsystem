
#include <iostream>
using namespace std;

class Object601
{
public:
	Object601(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout<<"object���캯�� ִ�� "<<"a"<<a<<" b "<<b<<endl;
	}
	~Object601()
	{
		cout<<"object�������� \n";
	}
protected:
	int a;
	int b;
};


class Parent601 : public Object601
{
public:
	Parent601(char *p) : Object601(1, 2)
	{
		this->p = p;
		cout<<"���๹�캯��..."<<p<<endl;
	}
	~Parent601()
	{
		cout<<"��������..."<<p<<endl;
	}

	void printP(int a, int b)
	{
		cout<<"���ǵ�..."<<endl;
	}

protected:
	char *p;
	
};


class child601 : public Parent601
{
public:
	child601(char *p) : Parent601(p), obj1(3, 4), obj2(5, 6)
	{
		this->myp = p;
		cout<<"����Ĺ��캯��"<<myp<<endl;
	}
	~child601()
	{
		cout<<"���������"<<myp<<endl;
	}
	void printC()
	{
		cout<<"���Ƕ���"<<endl;
	}
protected:
	char *myp;
	Object601 obj1;
	Object601 obj2;
};


void objplay()
{
	child601 c1("�̳в���");
}
void main601()
{
	objplay();
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}