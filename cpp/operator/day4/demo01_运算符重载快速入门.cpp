#include <iostream>;
using namespace std;
class Complex{
public:
	int a;
	int b; 
public:
	Complex(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
		 
}; 
Complex myAdd(Complex &c1, Complex &c2)
{
	Complex c3(c1.a + c2.a, c1.b + c2.b);
	return c3;
}

Complex operator+(Complex &c1, Complex &c2)
{
	Complex c3(c1.a + c2.a, c1.b + c2.b);
	return c3;
}
 
void main1()
{
	int a = 0, b = 0;
	int c;//基础数据类型 编译器知道如何运算
	c = a + b;
	
	Complex c1(1, 2), c2(3, 4);
	//c3 = c1 + c2; 
	Complex c4 = operator+(c1, c2);
	
	Complex c5 = c1 + c2;
	system("pause");
}