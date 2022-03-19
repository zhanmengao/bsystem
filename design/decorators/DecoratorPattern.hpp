#pragma once
#include<iostream>
#include <string>
using namespace std;
/*
装饰者模式：动态地给一个对象添加一些额外的职责。在增加功能上，这比生成子类更灵活

车本来只能跑，可以给它加上飞和游泳的功能
*/
class Car
{
public:
	virtual void showCar() = 0;
};

class BMW :public Car
{
public:
	virtual void showCar()
	{
		cout << "高档跑车" << endl;
	}
};
class flyCar :public Car
{
public:
	flyCar(Car *car)
	{
		this->car = car;
	}
	//新的功能
	void fly()
	{
		cout << "新增了飞的功能" << endl;
	}
	//虚函数重写
	virtual void showCar()
	{
		car->showCar();
		fly();
	}
private:
	//组合具体车
	Car *car;
};

//具体功能
class swimCar :public Car
{
public:
	swimCar(Car *car)
	{
		this->car = car;
	}
	//新的功能
	void swim()
	{
		cout << "新增了游泳的功能" << endl;
	}
	//虚函数重写
	virtual void showCar()
	{
		car->showCar();
		swim();
	}
private:
	//给我一辆车，我帮你组装
	Car *car;
};




void DecoratorMain()
{
	Car *runcar = new BMW;
	Car *flycar = new flyCar(runcar);
	Car *swimcar = new swimCar(flycar);

	runcar->showCar();;
	flycar->showCar();
	swimcar->showCar();
}