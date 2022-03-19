#include<iostream>
#include<string>
#include<vector>
#include<typeinfo>
using namespace std;

int main235()
{
	const int i = 42;
	auto j = i;//j是int型
	const auto &k = i;//k是const int的引用
	//k = 9;
	cout << typeid(k).name()<<endl;
	auto *p = &i;//p是const int *
	//*p = 9;
	cout << typeid(p).name() << endl;

	const auto j2 = i, &k2 = i;//j2是const int,k2是const int & 和i绑定
	cout << typeid(j2).name() << endl;
	cout << typeid(k2).name() << endl;
	//j2 = 9;
	//k2 = 9;

	return 0;
}