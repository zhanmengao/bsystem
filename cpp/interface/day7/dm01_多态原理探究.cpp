
#include <iostream>
using namespace std;

//��̬�������������� 
//Ҫ�м̳�  �麯����д  ����ָ��ָ��������� 

class Parent101
{
public:
	Parent101(int a = 0)
	{
		this->a = a;
	}

	virtual void print()  //1 ���ֽ�  дvirtal�ؼ��� �����⴦�� //�麯����
	{
		cout<<"���ǵ�"<<endl;
	}
	virtual void print2()  //1 ���ֽ�  дvirtal�ؼ��� �����⴦�� //�麯����
	{
		cout<<"���ǵ�"<<endl;
	}
private:
	int a;
};

class Child101 : public Parent101
{
public:
	Child101(int a = 0, int b = 0) :Parent101(a)
	{
		this->b = b;
	}

	virtual void print()
	{
		cout<<"���Ƕ���"<<endl;
	}
private:
	int b;
};

void HowToPlay1(Parent101 *base)
{
	base->print(); //�ж�̬����  //2 ���ֽ�  
	//Ч��:��������� ִ�������print���� ���������ִ�и����print���� 
	//C++��������������Ҫ������������� ���Ǹ������
	//���������������ֲ���vptrָ�� , ==>�麯����===>��������ڵ�ַ
	//�ٰ� (����ʱ��ʱ��,c++��������ȥ�ж�)
}

void main101()
{

	Parent101	p1; //3 ���ֽ� ��ǰ����  
				//���ඨ������ʱ�� C++���������ڶ��������һ��vptrָ�� 
	Child101	c1; //��������Ҳ��һ��vptrָ��

	HowToPlay1(&p1);
	HowToPlay1(&c1);
}