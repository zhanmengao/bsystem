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
		cout << "Ĭ�Ϲ��캯��"<<endl;
	}
	A(const A &obj)
	{
		cout << "�������캯��"<<endl;
	}
	~A()
	{
		cout << "��������"<<endl;
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