#include<iostream>;
using namespace std;

class A{
public :
	A(int a)
	{
		this->a = a;
	}
private:
	int a;
};

class B{
public:
	B(int n, int m) :a1(n), b(m)
	{
		//....
		
	}
private:
	A a1;
	int b;
};