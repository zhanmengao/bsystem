
#include <iostream>
using namespace std;

//����
class A801
{
public:
	A801()
	{
		cout<<"A�Ĺ��캯��"<<endl;
	}
public:
	
	static int a;
	int b;
	
public:
	void print()
	{
		cout<<"AAAAA "<<endl;
		cout << "b = " << b << endl;
		cout << "a = " << a << endl;
	}
	void SetA(int val)
	{
		a = val;
	}
protected:
private:
};

int A801::a = 100; //��仰 ���Ǽ򵥵ı�����ֵ ����Ҫ���� Ҫ����C++������ ��Ҫ���ҷ����ڴ� ,���ټ̳����� �õ���a ��Ȼ�ᱨ��..
                   //   ->>> static����������������Ҫ�����ⶨ��


class B801 : private A801
{

public:
	int b;
	int c;
public:
	void print()
	{
		cout << "BBBB " << endl;
		cout << "b = " << b << endl;
		cout << "a =" << a << endl;
	}
protected:
private:
};


//1 static�ؼ��� ����  ������ķ��ʿ��ƹ���

//2  ���Ǽ򵥵ı�����ֵ ����Ҫ���� Ҫ����C++������ ��Ҫ���ҷ����ڴ� ,���ټ̳����� �õ���a ��Ȼ�ᱨ��..

//3 A������ӹ��캯�� 
	//A��Ĺ��캯����   A�Ĺ��캯����˽�еĹ��캯�� ... 
	//�������̳�ҪС��....
	//�������� .... UML

void main801()
{
	A801 a1;
	a1.print();
	a1.SetA(999);
	B801 b1;
	b1.print();      //b1��a1�õ���ͬһ��static����a
	system("pause");
}

void main802()
{
	B801  b1;
	//b1.a = 200; //error ���ɷ���
	system("pause");
}
