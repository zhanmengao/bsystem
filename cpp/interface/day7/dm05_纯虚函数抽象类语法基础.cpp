
#include <iostream>
using namespace std;

////�����������(����һ��Ԥ�ȶ���õĽӿڱ��)

//����� ....ģ��Ļ���


class  Figure //������
{
public:
	//�Ķ�һ��ͳһ�Ľ���(�ӿ�),������ʹ��,���������ȥʵ��
	virtual void getArea() = 0 ; //���麯��
protected:
private:
};

class Circle : public Figure
{
public:
	Circle(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual void getArea()
	{
		cout<<"Բ�ε����: "<<3.14*a*a<<endl;;

	}

private:
	int a;
	int b;
};

class Tri : public Figure
{
public:
	Tri(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual void getArea() 
	{
		cout<<"�����ε����: "<<a*b/2<<endl;;
	}

private:
	int a;
	int b;
};


class Square : public Figure
{
public:
	Square(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual void getArea() 
	{
		cout<<"�ı��ε����: "<<a*b<<endl;;
	}

private:
	int a;
	int b;
};


void objplay(Figure *base)
{
	base->getArea(); //�ᷢ����̬
}


void main501()
{

	//Figure f; //�����಻�ܱ�ʵ����

	Figure *base = NULL; //�����಻�ܱ�ʵ����

	Circle c1(10, 20);
	Tri t1(20, 30);
	Square s1(50, 60);

	//�����������(����һ��Ԥ�ȶ���õĽӿڱ��)

	objplay(&c1);
	objplay(&t1);
	objplay(&s1);

	base = &c1;

	objplay(base);
	//c1.getArea();
}