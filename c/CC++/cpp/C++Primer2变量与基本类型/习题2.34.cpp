#include<iostream>
#include<string>
#include<vector>
#include<typeinfo>
using namespace std;

int main234()
{
	int i = 0, &r = i;
	auto a = r;//a是int，用r的值来进行初始化，r是i的别名
	a = 42;
	cout << a<<typeid(a).name()<<endl;
	
	const int ci = i, &cr = ci;//ci是const int，cr是ci的别名
	auto b = ci;//b是int，用ci的值进行初始化
	b = 42;
	cout << b<<typeid(b).name() << endl;
	auto c = cr;//c是int，用cr的值进行初始化，cr是ci的别名
	c = 42;
	cout << typeid(c).name() << endl;
	auto d = &i;//d是int *，指向i的地址
	d = const_cast<int *>(&cr);
	cout << typeid(d).name() << endl;
	auto e = &ci;//e是int const * 
	cout << typeid(e).name() << endl;

	//引用
	auto &g = ci;//g是常引用 const int &
	cout << typeid(g).name() << endl;
	const auto &j = 42;//j是常引用 const int &
	cout << typeid(j).name() << endl;
	return 0;
}
