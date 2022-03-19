#pragma once

//编译时多态：函数重载
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

//运行时多态：虚函数
class Shape                     // 形状类
{
public:
	virtual double calcArea()
	{
		
	}
	virtual ~Shape()
	{

	}
};
class Circle : public Shape     // 圆形类
{
public:
	Circle(double num)
	{

	}
	virtual double calcArea()
	{

	}
};
class Rect : public Shape       // 矩形类
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
	shape1->calcArea();         // 调用圆形类里面的方法
	shape2->calcArea();         // 调用矩形类里面的方法
	delete shape1;
	shape1 = nullptr;
	delete shape2;
	shape2 = nullptr;
	return 0;
}