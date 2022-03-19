//类的继承方式对子类对外访问属性影响

#include <cstdlib>
#include <iostream>

using namespace std;

class A301
{
private:
	int a;
protected:
	int b;
public:
	int c;

	A301()
	{
		a = 0;		b = 0;		c = 0;
	}

	void set(int a, int b, int c)
	{
		this->a = a;		this->b = b;		this->c = c;
	}
};
class B301 : public A301
{
public:
	void print()
	{
		//cout<<"a = "<<a; //err
		cout<<"b = "<<b; //ok
		cout<<"c = "<<endl; //ok
	}
};

class C301 : protected A301
{
public:
	void print()
	{
		//cout<<"a = "<<a; //err
		cout<<"b = "<<b; // ok
		cout<<"c = "<<endl; //ok
	}
};

class D301 : private A301
{
public:
	void print()
	{
		//cout<<"a = "<<a; //err
		cout<<"b = "<<b<<endl;  //ok
		cout<<"c = "<<c<<endl; // ok
	}
};

int main301()
{
	
	A301 aa;
	B301 bb;
	C301 cc;
	D301 dd;

	aa.c = 100;  //ok
	bb.c = 100; // ok
	//cc.c = 100; // err 保护的
	//dd.c = 100;  // err
	
	aa.set(1, 2, 3); //ok
	bb.set(10, 20, 30); //ok
	//cc.set(40, 50, 60); // err
	//dd.set(70, 80, 90);  //err
	

	bb.print(); //ok
	cc.print(); //ok
	dd.print(); //


	/**/
	system("pause");
	return 0;
}
