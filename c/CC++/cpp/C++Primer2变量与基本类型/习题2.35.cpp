#include<iostream>
#include<string>
#include<vector>
#include<typeinfo>
using namespace std;

int main235()
{
	const int i = 42;
	auto j = i;//j��int��
	const auto &k = i;//k��const int������
	//k = 9;
	cout << typeid(k).name()<<endl;
	auto *p = &i;//p��const int *
	//*p = 9;
	cout << typeid(p).name() << endl;

	const auto j2 = i, &k2 = i;//j2��const int,k2��const int & ��i��
	cout << typeid(j2).name() << endl;
	cout << typeid(k2).name() << endl;
	//j2 = 9;
	//k2 = 9;

	return 0;
}