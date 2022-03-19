#include<iostream>
using namespace std;
#include "Complex.h"


template<class T>
Complex<T>& Complex<T>::operator-(const Complex<T> &obj)
{
	this->a = this->a - obj.a;
	this->b = this->b - obj.b;
	return *this;
}
//重载函数提出来
template<class T>
Complex<T>& Complex<T>::operator+(const Complex<T> &obj)
{
	this->a = this->a + obj.a;
	this->b = this->b + obj.b;
	return *this;
}


//构造函数提出来
template<class T>
Complex<T>::Complex(T a, T b)
{
	this->a = a;
	this->b = b;
}
//普通函数提出来；
template<class T>
void Complex<T>::printCom()
{
	cout << a << "+" << b << "i" << endl;
}

//友元重载提出来
template<class T>
ostream& operator<<(ostream &out, const Complex<T> &obj)
{
	cout << "a=" << obj.a << "b=" << obj.b << endl;
	return out;
}


