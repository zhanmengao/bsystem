
#include <iostream>
using namespace std;

/*
class ostream
{

};
*/

class Complex
{
private:
	int a;
	int b;
	//friend void operator<<(ostream &out, Complex &c1);
	friend ostream& operator<<(ostream &out, Complex &c1);

public:
	Complex(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	void printCom()
	{
		cout<<a<<" + " << b << "i" <<endl;
	}
public:

	//ʵ�� + ���������
	Complex operator+(Complex &c2)
	{
		Complex tmp(a + c2.a, b + c2.b);
		return tmp;
	}

	//ǰ��++
	Complex& operator++()
	{
		a++;
		b++;
		return *this;
	}

	//����++
	Complex operator++(int)
	{
		//��ʹ�� ����c1�Ӽ�
		Complex tmp = *this;
		//return c1;
		this->a ++;
		this->b ++;
		return tmp;
	}
	//��Ա������ ʵ�� -���������
	 Complex operator-(Complex &c2)
	{
		Complex tmp(this->a - c2.a, this->b - c2.b);
		return tmp;
	}

	 //ǰ��--
	Complex& operator--()
	{
		this->a --;
		this->b --;
		return *this;
	}

	//����--
	Complex operator--(int)
	{
		Complex tmp = *this;
		this->a--;
		this->b--;
		return tmp;
	}
};


void main301()
{
	Complex c1(1, 2), c2(3, 4);

	//1	ȫ�ֺ����� ʵ�� + ���������
	// Complex operator+(Complex &c1, Complex &c2);
	Complex c3 = c1 + c2;
	c3.printCom();

	//2 ��Ա���� �� ʵ�� -���������
	//c1.operator-(c2);
	//Complex operator-(Complex &c2)
	Complex c4 = c1 - c2;
	c4.printCom();

	//ǰ��++������ ��ȫ�ֺ���ʵ��
	++c1;
	c1.printCom();

	//ǰ��--������ ��Ա��������
	--c1;
	c1.printCom();
	//Complex& operator++(Complex &c1)
	//c1.operator--();

	//����++������ ��ȫ�ֺ���ʵ��
	c1++;
	c1.printCom();

	//����--������ �ó�Ա����ʵ��
	c1--;
	c1.printCom();
	//c1.operator--()
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

/*
void operator<<(ostream &out, Complex &c1)
{
	out<<"12345 �������ǿ�"<<endl;
	out<<c1.a << " + " << c1.b << "i" << endl;
}
*/

ostream& operator<<(ostream &out, Complex &c1)
{
	out<<"12345 �������ǿ�"<<endl;
	out<<c1.a << " + " << c1.b << "i" << endl;
	return out;
}

void main302()
{
	int a = 10;
	Complex c1(1, 2), c2(3, 4);
	cout<<a<<endl; //������������ 

	//1
	cout << c1 ;
	//2 ostream ���� ��� ��Ա���� .operator<<
	//ostream
	//cout.operator<<(c1);

	//2 ��������ֵ����ֵ ��Ҫ����һ������
	cout << c1  << "aaddddd";
	//
	//cout.operator<<(c1) .operator<<("aaddddd");
	//void.operator<<("aaddddd");

	system("pause");
}