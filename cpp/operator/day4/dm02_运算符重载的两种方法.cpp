
#include <iostream>
using namespace std;

class Complex
{
private:
	int a;
	int b;
	//ȫ�ֺ��� ����+�����
	friend Complex operator+(Complex &c1, Complex &c2);
	//���� ǰ��++
	friend Complex& operator++(Complex &c1);
	friend Complex operator++(Complex &c1, int);
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

//ȫ�ֺ����� ʵ�� + ���������
Complex operator+(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a + c2.a, c1.b + c2.b);
	return tmp;
}

//ǰ��++
Complex& operator++(Complex &c1)
{
	c1.a++;
	c1.b++;
	return c1;
}

//����++��ռλ������
Complex operator++(Complex &c1, int)
{
	//��ʹ�� ����c1�Ӽ�
	Complex tmp = c1;
	//return c1;
	c1.a ++;
	c1.b ++;
	return tmp;
}

/*
ȫ�ֺ��������Ա��������ʵ����������ز���
	1��Ҫ���ϲ�����������һ��������д����������
	2�����ݲ�������д���������� 
	3������ҵ�����ƺ�������ֵ(�������Ƿ������� ����ָ�� Ԫ��)����ʵ�ֺ���ҵ��
*/
void main201()
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