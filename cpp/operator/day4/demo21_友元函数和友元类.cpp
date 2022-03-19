#include <iostream>
using namespace std;

class A{
public :
	friend void modifyA(A *);
	//B类是A类的友元类
	friend class B;
	A()
	{

	}
	A(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	int getA()
	{
		return this->a;
	}
private:
	int a;
	int b;
};
class B{
public:
	void set(int i)
	{
		a1.a = 1000;
		a1.b = i;
	}
	void printfB()
	{
		cout << a1.a << a1.b << endl;
	}
private:
	A a1;
};

void modifyA(A *pa)
{
	pa->a = 100;
}

int main4()
{
	A a(1, 2);
	//cout << "a=" << a.getA() << endl;
	//modifyA(&a);
	B b1;
	b1.set(300);
	b1.printfB();
	//cout << "a=" << a.getA() << endl;
	system("pause");
	return 0;
}