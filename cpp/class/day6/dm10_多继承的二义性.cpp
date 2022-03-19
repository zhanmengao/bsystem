
#include <iostream>
using namespace std;

class B
{
public:
	int b;
protected:
private:

};

class B1 : virtual public B
{
public:
	int b1;

};

class B2 : virtual public B
{
public:
	int b2;
};

class C : public B1, public B2    
{
public:

	int c;
};

void main1001()
{

	C  c1;
	c1.b1 = 100;
	c1.b2 = 200;
	c1.c = 300;

	c1.b = 500; //继承的二义性 和 虚继承解决方案  ---->>>虚继承机制使得C类只继承一个来自于B的变量b

	
	//1、如果不适用虚继承，则C里面有两个来自B2的变量b

	//2、基类的多个分支必须都是虚继承
	//c1.B1::b = 500;
	//c1.B2::b = 500;
}