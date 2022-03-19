
#include <iostream>
using namespace std;

class Car
{
public:
	virtual void show() = 0;
};

class RunCar : public Car 
{
public:
	virtual void show()
	{
		cout << "������" << endl;
	}
protected:
private:
};


class SwimCarDirector : public Car
{
public:
	SwimCarDirector(Car *car)
	{
		m_car = car;
	}
	void swim()
	{
		cout << "������" << endl;
	}
	virtual void show()
	{
		m_car->show();
		swim();
	}
protected:
private:
	Car *m_car;
};

class  FlyCarDirector : public Car
{
public:
	FlyCarDirector(Car *car)
	{
		m_car = car;
	}
	void fly()
	{
		cout << "���Է�" << endl;
	}
	virtual void show()
	{
		m_car->show();
		fly();
	}

private:
	Car *m_car;
};

void main()
{
	Car * mycar = NULL;
	mycar = new RunCar;
	printf("-----000\n");
	mycar->show();

	printf("-----aaa\n");

	FlyCarDirector *flycar = new FlyCarDirector(mycar);
	flycar->show();

	printf("-----bbb\n");
	SwimCarDirector *swimcar = new SwimCarDirector(flycar);
	swimcar->show();
	
	delete swimcar;
	delete flycar;
	delete mycar;
	system("pause");
	return ;
}