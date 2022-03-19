#include<iostream>
using namespace std;//c++的命名空间
class circle
{
public:
	double r;
	double pi = 3.1415926;
	double area = pi*r*r;
};

// 2010编译不通过 但是在2013编译器能编译通过
int main202()
{
	circle c1;
	cout << "please input your r" << endl;
	cin >> c1.r;

	cout << c1.area << endl;	//乱码

	system("pause");
	return 0;
}