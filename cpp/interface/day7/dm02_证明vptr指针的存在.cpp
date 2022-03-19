
#include <iostream>
using namespace std;


class Parent201
{
public:
	Parent201(int a = 0)
	{
		this->a = a;
	}

	void print() 
	{
		cout<<"我是爹"<<endl;
	}
private:
	int a;
};

class Parent202
{
public:
	Parent202(int a = 0)
	{
		this->a = a;
	}

	virtual void print()  
	{
		cout<<"我是爹"<<endl;
	}
private:
	int a;
};

void main201()
{
	printf("sizeof(Parent):%d sizeof(Parent2):%d \n", sizeof(Parent201), sizeof(Parent202));
}
