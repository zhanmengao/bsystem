
#include <iostream>
using namespace std;

/*
class ostream
{

};
*/

class Complex
{
private:
	int a;
	int b;
	//friend void operator<<(ostream &out, Complex &c1);
	friend ostream& operator<<(ostream &out, Complex &c1);

public:
	Complex(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	void printCom()
	{
		cout<<a<<" + " << b << "i" <<endl;
	}
public:

	//实现 + 运算符重载
	Complex operator+(Complex &c2)
	{
		Complex tmp(a + c2.a, b + c2.b);
		return tmp;
	}

	//前置++
	Complex& operator++()
	{
		a++;
		b++;
		return *this;
	}

	//后置++
	Complex operator++(int)
	{
		//先使用 在让c1加加
		Complex tmp = *this;
		//return c1;
		this->a ++;
		this->b ++;
		return tmp;
	}
	//成员函数法 实现 -运算符重载
	 Complex operator-(Complex &c2)
	{
		Complex tmp(this->a - c2.a, this->b - c2.b);
		return tmp;
	}

	 //前置--
	Complex& operator--()
	{
		this->a --;
		this->b --;
		return *this;
	}

	//后置--
	Complex operator--(int)
	{
		Complex tmp = *this;
		this->a--;
		this->b--;
		return tmp;
	}
};


void main301()
{
	Complex c1(1, 2), c2(3, 4);

	//1	全局函数法 实现 + 运算符重载
	// Complex operator+(Complex &c1, Complex &c2);
	Complex c3 = c1 + c2;
	c3.printCom();

	//2 成员函数 法 实现 -运算符重载
	//c1.operator-(c2);
	//Complex operator-(Complex &c2)
	Complex c4 = c1 - c2;
	c4.printCom();

	//前置++操作符 用全局函数实现
	++c1;
	c1.printCom();

	//前置--操作符 成员函数方法
	--c1;
	c1.printCom();
	//Complex& operator++(Complex &c1)
	//c1.operator--();

	//后置++操作符 用全局函数实现
	c1++;
	c1.printCom();

	//后置--操作符 用成员函数实现
	c1--;
	c1.printCom();
	//c1.operator--()
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

/*
void operator<<(ostream &out, Complex &c1)
{
	out<<"12345 生活真是苦"<<endl;
	out<<c1.a << " + " << c1.b << "i" << endl;
}
*/

ostream& operator<<(ostream &out, Complex &c1)
{
	out<<"12345 生活真是苦"<<endl;
	out<<c1.a << " + " << c1.b << "i" << endl;
	return out;
}

void main302()
{
	int a = 10;
	Complex c1(1, 2), c2(3, 4);
	cout<<a<<endl; //按照数据类型 

	//1
	cout << c1 ;
	//2 ostream 类中 添加 成员函数 .operator<<
	//ostream
	//cout.operator<<(c1);

	//2 函数返回值当左值 需要返回一个引用
	cout << c1  << "aaddddd";
	//
	//cout.operator<<(c1) .operator<<("aaddddd");
	//void.operator<<("aaddddd");

	system("pause");
}