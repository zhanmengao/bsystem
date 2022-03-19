#include <iostream>
using namespace std;

class Parent101
{
public:
	void print()
	{
		a = 0;
		b = 0;
		cout<<"a"<<a<<endl;
		cout<<"b"<<b<<endl;
	}
	
	int b;
protected:
private:
	int a;
};

//class Child : private Parent
//class Child :  protected Parent public
class Child101 :  public Parent101
{
public:
protected:
private:
	int c;
};

void main101()
{
	
	Child101 c1;
	//c1.c = 1;
	//c1.a = 2;
	c1.b = 3;

	c1.print();
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}
