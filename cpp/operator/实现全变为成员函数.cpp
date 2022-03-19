#include <iostream>;
using namespace std;


class Complex3{
private:
	int a;
	int b;
public:
	friend ostream& operator<<(ostream &out, Complex3 &c1);
	Complex3(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
	//成员函数方法
	Complex3 operator-(Complex3 &c)
	{
		return Complex3(this->a - c.a, this->b - c.b);
	}
	//--c
	Complex3& operator--()
	{
		this->a--;
		this->b--;
		return *this;
	}
	//c--
	Complex3 operator--(int)
	{
		Complex3 tmp = *this;
		this->a--;
		this->b--;
		return tmp;

	}
	//c++
	Complex3 operator++(int)
	{
		Complex3 tmp = *this;
		this->a++;
		this->b++;
		return tmp;
	};
	//++c
	Complex3& operator++()
	{
		this->a++;
		this->b++;
		return *this;
	}
	void print()
	{
		cout << this->a << "  " << this->b << endl;
	}

};

ostream& operator<<(ostream &out, Complex3 &c1)
{
	out << c1.a << "+" << c1.b <<"i"<<endl;
	return out;
}


void main333()
{
	int a = 10;
	Complex3 c1(1, 2), c2(3, 4);
	cout << a << endl;
	cout << c1 << "aaaaa";

	system("pause");
}