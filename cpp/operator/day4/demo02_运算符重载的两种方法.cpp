#include <iostream>;
using namespace std;
class Complex2{
private:
	int a;
	int b;
	friend Complex2& operator++(Complex2 &c1);
	friend Complex2 operator+(Complex2 &c1, Complex2 &c2);
	friend Complex2 operator++(Complex2 &c1, int);
public:
	Complex2(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
	//成员函数方法
	Complex2 operator-(Complex2 &c)
	{
		return Complex2(this->a - c.a, this->b - c.b);
	}
	Complex2& operator--()
	{
		this->a--;
		this->b--;
		return *this;
	}
	Complex2 operator--(int)
	{
		Complex2 tmp = *this;
		this->a--;
		this->b--;
		return tmp;

	}
	void print()
	{ 
		cout << this->a << "  " << this->b << endl;
	}

};

//友元函数方法
//++c
Complex2& operator++(Complex2 &c1)
{
	c1.a++;
	c1.b++;
	return c1;
}
//c++
Complex2 operator++(Complex2 &c1,int)
{
	Complex2 tmp = c1;
	c1.a++;
	c1.b++;
	return tmp;
}

void main2()
{

	Complex2 c1(1, 2), c2(3, 4); 
	++c1;
	c1.print();
	--c1;
	c1.print(); 
	c1++;
	c1.print();
	c1--;
	c1.print();
	system("pause");
}