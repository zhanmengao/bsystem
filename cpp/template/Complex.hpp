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
//���غ��������
template<class T>
Complex<T>& Complex<T>::operator+(const Complex<T> &obj)
{
	this->a = this->a + obj.a;
	this->b = this->b + obj.b;
	return *this;
}


//���캯�������
template<class T>
Complex<T>::Complex(T a, T b)
{
	this->a = a;
	this->b = b;
}
//��ͨ�����������
template<class T>
void Complex<T>::printCom()
{
	cout << a << "+" << b << "i" << endl;
}

//��Ԫ���������
template<class T>
ostream& operator<<(ostream &out, const Complex<T> &obj)
{
	cout << "a=" << obj.a << "b=" << obj.b << endl;
	return out;
}


