#include<iostream>
using namespace std;

class Parent3{
public:
	Parent3(int a)
	{
		this->a = a;
		cout << "a" << a << endl;
	}
	void printF()
	{
		cout << "Parent" << endl;
	}
protected:

private:
	int a;
};

class Child : public Parent3{
public:
	Child(int b) :Parent3(b)
	{
		this->b = b;
		cout << "b" << b << endl;
	}
private:
	int b;
};
