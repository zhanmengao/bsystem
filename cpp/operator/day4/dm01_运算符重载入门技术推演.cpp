
#include <iostream>
using namespace std;

class Complex
{
public:
	int a;
	int b;
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
};

//1 ������ȫ�ֺ���
Complex myAdd(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a + c2.a, c1.b+ c2.b);
	return tmp; //
}

//2 ������ ����
Complex operator+(Complex &c1, Complex &c2)
{
	cout<<"12345��ɽ ���ϻ�"<<endl;
	Complex tmp(c1.a + c2.a, c1.b+ c2.b);
	return tmp; //
}

Complex operator/(Complex &c1, Complex &c2)
{
	cout << "12345��ɽ ���ϻ�" << endl;
	Complex tmp(c1.a / c2.a, c1.b / c2.b);
	return tmp; //
}

void main101()
{
	
	int a = 0, b = 0;
	int c;
	c = a + b; //1 ������������ �������Ѿ�֪����. �������

	// a + bi �����������
	Complex c1(1, 2), c2(3, 4);
	Complex c3; //2 �� Ҳ��һ����������  �û��Զ����������� C++������ �ǲ�֪����ν�������
	//c3 = c1 + c2 ;
	//c1--; --c1

	//3 c++������Ӧ�ø����ǳ���Ա�ṩһ�ֻ��� ... 
	//���Զ����������� �л��� ���� ��������� ====> ��������ػ��� 

	//4 ��������ػ���

	//����1 
	//Complex c4 = myAdd(c1, c2);
	//c4.printCom();

	//����2  //Complex c4  = c1 + c2
	//Complex c4 = operator+(c1, c2);
	//c4.printCom();

	//����3 
	Complex c4 = c1 + c2;
	c4.printCom();
	
	//����3 
	Complex c4 = c1 / c2;
	c4.printCom();

	//�ܽ�: 1 ��������صı��� �� �������� 



	cout<<"hello..."<<endl;
	system("pause");
	return ;
}