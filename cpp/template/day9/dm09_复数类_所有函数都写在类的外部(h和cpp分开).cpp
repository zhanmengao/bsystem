
#include <iostream>
using namespace std;
namespace demo09
{
	template <typename T>
	class Complex
	{
		friend Complex<T> MySub(Complex<T> &c1, Complex<T> &c2);
		friend ostream & operator<<<T> (ostream &out, Complex<T> &c3);    //记得在函数后面加上<T>

	public:
		Complex(T a, T b);
		void printCom();
		Complex operator+ (Complex &c2);

	private:
		T	a;
		T	b;
	};

	//构造函数的实现 写在了类的外部
	template <typename T>
	Complex<T>::Complex<T>(T a, T b)
	{
		this->a = a;
		this->b = b;
	}

	template <typename T>
	void Complex<T>::printCom()
	{
		cout << "a:" << a << " b: " << b << endl;
	}

	//本质是 : 模板是两次 编译生成的 第一次生成的函数头 和第二次生成的函数头 不一样
	//成员函数 实现 +运算符重载
	template <typename T>
	Complex<T>  Complex<T>::operator+(Complex<T> &c2)
	{
		Complex tmp(a + c2.a, b + c2.b);
		return tmp;
	}


	template <typename T>
	ostream & operator<<(ostream &out, Complex<T> &c3)
	{
		out << c3.a << " + " << c3.b << "i" << endl;
		return out;
	}

	//////////////////////////////////////////////////////////////////////////

	//滥用 友元函数
	template <typename T>
	Complex<T> MySub(Complex<T> &c1, Complex<T> &c2)
	{
		Complex<T> tmp(c1.a - c2.a, c1.b - c2.b);
		return tmp;
	}
}

using namespace demo09;
void main901()
{
	//需要把模板类 进行具体化以后  才能定义对象  C++编译器要分配内存
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//滥用友元函数
	{
		//Complex<int> c4 = MySub<int>(c1, c2);
		//cout << c4 << endl;

	}
}