
#include <iostream>
using namespace std;

namespace demo08
{


	template <typename T>
	class Complex; //类的前置声明

	template <typename T>
	Complex<T> MySub(Complex<T> &c1, Complex<T> &c2);


	template <typename T>
	class Complex
	{
		friend Complex<T> MySub<T>(Complex<T> &c1, Complex<T> &c2);

		friend ostream & operator<< <T> (ostream &out, Complex &c3);

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
	Complex<T>::Complex(T a, T b)
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
	Complex<T>  Complex<T>::operator+ (Complex<T> &c2)
	{
		Complex tmp(a + c2.a, b + c2.b);
		return tmp;
	}

	//友元函数 实现 << 运算符重载

	/*
	1>dm08_复数类_所有函数都写在类的外部(在一个cpp中).obj : error LNK2019: 无法解析的外部符号 "
	class std::basic_ostream<char,struct std::char_traits<char> > &
	__cdecl operator<<
	(class std::basic_ostream<char,struct std::char_traits<char> > &,class Complex<int> &)"
	(??6@YAAAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AAV01@AAV?$Complex@H@@@Z)，
	该符号在函数 _main 中被引用
	1>E:\01-work\21_CPlusPlus课程\day09\泛型编程课堂操练\Debug\泛型编程课堂操练.exe : fatal error LNK1120: 1 个无法解析的外部命令
	*/

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

using namespace demo08;
void main801()
{
	//需要把模板类 进行具体化以后  才能定义对象  C++编译器要分配内存
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//滥用友元函数
	{
		Complex<int> c4 = MySub<int>(c1, c2);
		cout << c4 << endl;

	}
}