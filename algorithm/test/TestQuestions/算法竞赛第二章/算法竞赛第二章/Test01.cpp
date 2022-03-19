#include<iostream>
#include <string>
using namespace std;

void main919()
{
	int i = 10, &r1 = i;
	double d = 0, &r2 = d;
	r2 = 3.1415926;
	r2 = r1;
	i = r2;
	r1 = d;

}
class A{
public:
	A()
	{
		cout << "默认构造函数"<<endl;
	}
	A(const A &obj)
	{
		cout << "拷贝构造函数"<<endl;
	}
	~A()
	{
		cout << "析构函数"<<endl;
	}
};
A getA()
{
	A a;
	return a;
}
void main()
{
	A a = getA();

}