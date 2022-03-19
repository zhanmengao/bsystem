#pragma  once

#include <iostream>
using namespace std;
namespace demo09
{
	template <typename T>
	class Complex
	{
		//friend Complex<T> MySub (Complex<T> &c1, Complex<T> &c2);
		friend ostream & operator<< <T> (ostream &out, Complex &c3);

	public:
		Complex(T a, T b);
		void printCom();
		Complex operator+ (Complex &c2);

	private:
		T	a;
		T	b;
	};
}
