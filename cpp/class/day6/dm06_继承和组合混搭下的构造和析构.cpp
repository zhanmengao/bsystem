
#include <iostream>
using namespace std;

class Object601
{
public:
	Object601(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout<<"object构造函数 执行 "<<"a"<<a<<" b "<<b<<endl;
	}
	~Object601()
	{
		cout<<"object析构函数 \n";
	}
protected:
	int a;
	int b;
};


class Parent601 : public Object601
{
public:
	Parent601(char *p) : Object601(1, 2)
	{
		this->p = p;
		cout<<"父类构造函数..."<<p<<endl;
	}
	~Parent601()
	{
		cout<<"析构函数..."<<p<<endl;
	}

	void printP(int a, int b)
	{
		cout<<"我是爹..."<<endl;
	}

protected:
	char *p;
	
};


class child601 : public Parent601
{
public:
	child601(char *p) : Parent601(p), obj1(3, 4), obj2(5, 6)
	{
		this->myp = p;
		cout<<"子类的构造函数"<<myp<<endl;
	}
	~child601()
	{
		cout<<"子类的析构"<<myp<<endl;
	}
	void printC()
	{
		cout<<"我是儿子"<<endl;
	}
protected:
	char *myp;
	Object601 obj1;
	Object601 obj2;
};


void objplay()
{
	child601 c1("继承测试");
}
void main601()
{
	objplay();
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}