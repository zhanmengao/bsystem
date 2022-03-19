#include<iostream>
#include<string>
#include<vector>
#include<typeinfo>
using namespace std;

int main238()
{
	//区别2
	const int a = 42;
	auto b = a;
	cout << typeid(b).name()<<endl;
	decltype(a) c = a;
	cout << typeid(c).name() << endl;
	b = 9;
	//c = 9;

	//区别3
	int d = 9;
	decltype((d)) e = d;
	e = 10;
	cout << "d=" << d << "e="<<e<<endl;
	cout << &d << "  " << &e<<endl;
	return 0;
}