
#include <iostream>
using namespace std;

class A
{
public:
	A(int _a)
	{
		a = _a;
		cout << "���캯��" << "a" << a << endl;
	}

	~A()
	{
		cout << "��������" << "a" << a << endl;
	}

protected:
private:
	int a;
};


//1 ���캯���ĳ�ʼ���б�  ���: ��B���� �����һ�� A����� (A������˹��캯��)
//���ݹ��캯���ĵ��ù��� ���A�Ĺ��캯��, ����Ҫ��;û�л����ʼ��A
//�µ��﷨  Constructor::Contructor() : m1(v1), m2(v1,v2), m3(v3)
class B
{
public:
	B(int _b1, int _b2) : a1(1), a2(2), c(0)
	{

	}

	B(int _b1, int _b2, int m, int n) : a1(m), a2(n), c(0)
	{
		b1 = _b1;
		b2 = _b2;
		cout <<"B�Ĺ��캯��"<<endl;
	}
	~B()
	{
		cout<<"B����������" <<endl;
	}

protected:
private:
	int b1;
	int b2;
	A a2;
	A a1;
	const int c;
};


//2 ��ִ�� ����϶���Ĺ��캯�� 
//�����϶����ж��,���ն���˳��, �����ǰ��ճ�ʼ���б��˳��

//�������� : �͹��캯���ĵ���˳���෴

//3 ����϶���Ĺ���˳�� �붨��˳���й�ϵ ,���ʼ���б��˳��û�й�ϵ.
//4 ��ʼ���б� ���� ��const ���Ը�ֵ 
//5 �ڳ�ʼ���б��жԳ�Ա�������г�ʼ�����������ĳ�ʼ�����ڹ��캯���н��е�ֻ�Ǹ�ֵ -���ڳ�ʼ���б��н��г�ʼ��Ч�ʻ��
void obj10play()
{
	
	//A a1(10);
	//B ojbB(1, 2);

	//1�������� 
	B ojbB2(1, 2,3, 4);

	//2 ����˳��
	
	return ;
}

void main100()
{
	obj10play();
	system("pause");
}