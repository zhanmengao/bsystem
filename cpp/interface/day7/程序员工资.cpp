#include<iostream>
using namespace std;


class programmer1{
public:
	virtual float getSalary() = 0;
};

class JuniorProgrammer1 :public programmer1{
public:
	virtual float getSalary()
	{
		return 8000;
	}
};

class IntermediateProgrammer1 :public programmer1{
public:
	virtual float getSalary()
	{
		return 15000;
	}
};

class seniorProgrammer1 :public programmer1{
public:
	virtual float getSalary()
	{
		return 30000;
	}
};

class architect1 :public programmer1{
public:
	virtual float getSalary()
	{
		return 80000;
	}
};

void main1()
{
	programmer1 *it;
	JuniorProgrammer1 di;
	IntermediateProgrammer1 zhong;
	seniorProgrammer1 gao;
	architect1 jiagou;
	it = &zhong;
	cout << di.getSalary() << endl;

}