#pragma  once
#include<iostream>
using namespace std;


template<class T>
class Complex
{
public:
	Complex(T a, T b);
	void printCom();
	Complex& operator+(const Complex &obj);
	friend ostream& operator<< <T>(ostream &out, const Complex<T> &obj);
	Complex& operator-(const Complex &obj);

protected:
private:
	T a;
	T b;
};