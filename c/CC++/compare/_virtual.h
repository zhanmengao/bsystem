#pragma once

//����ʱ��̬����������
class A
{
public:
	void doThing(int a)
	{

	}
	void doThing(int a, int b)
	{

	}
};

//����ʱ��̬���麯��
class Shape                     // ��״��
{
public:
	virtual double calcArea()
	{
		
	}
	virtual ~Shape()
	{

	}
};
class Circle : public Shape     // Բ����
{
public:
	Circle(double num)
	{

	}
	virtual double calcArea()
	{

	}
};
class Rect : public Shape       // ������
{
public:
	Rect(double nWidth, double nHeight)
	{

	}
	virtual double calcArea()
	{

	}
};
int mainVirtual()
{
	Shape * shape1 = new Circle(4.0);
	Shape * shape2 = new Rect(5.0, 6.0);
	shape1->calcArea();         // ����Բ��������ķ���
	shape2->calcArea();         // ���þ���������ķ���
	delete shape1;
	shape1 = nullptr;
	delete shape2;
	shape2 = nullptr;
	return 0;
}