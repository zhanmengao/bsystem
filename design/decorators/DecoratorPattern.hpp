#pragma once
#include<iostream>
#include <string>
using namespace std;
/*
װ����ģʽ����̬�ظ�һ���������һЩ�����ְ�������ӹ����ϣ����������������

������ֻ���ܣ����Ը������Ϸɺ���Ӿ�Ĺ���
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
		cout << "�ߵ��ܳ�" << endl;
	}
};
class flyCar :public Car
{
public:
	flyCar(Car *car)
	{
		this->car = car;
	}
	//�µĹ���
	void fly()
	{
		cout << "�����˷ɵĹ���" << endl;
	}
	//�麯����д
	virtual void showCar()
	{
		car->showCar();
		fly();
	}
private:
	//��Ͼ��峵
	Car *car;
};

//���幦��
class swimCar :public Car
{
public:
	swimCar(Car *car)
	{
		this->car = car;
	}
	//�µĹ���
	void swim()
	{
		cout << "��������Ӿ�Ĺ���" << endl;
	}
	//�麯����д
	virtual void showCar()
	{
		car->showCar();
		swim();
	}
private:
	//����һ�������Ұ�����װ
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