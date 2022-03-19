
#include <iostream>
using namespace std;

//单例
class A801
{
public:
	A801()
	{
		cout<<"A的构造函数"<<endl;
	}
public:
	
	static int a;
	int b;
	
public:
	void print()
	{
		cout<<"AAAAA "<<endl;
		cout << "b = " << b << endl;
		cout << "a = " << a << endl;
	}
	void SetA(int val)
	{
		a = val;
	}
protected:
private:
};

int A801::a = 100; //这句话 不是简单的变量赋值 更重要的是 要告诉C++编译器 你要给我分配内存 ,我再继承类中 用到了a 不然会报错..
                   //   ->>> static变量在类内声明后，要在类外定义


class B801 : private A801
{

public:
	int b;
	int c;
public:
	void print()
	{
		cout << "BBBB " << endl;
		cout << "b = " << b << endl;
		cout << "a =" << a << endl;
	}
protected:
private:
};


//1 static关键字 遵守  派生类的访问控制规则

//2  不是简单的变量赋值 更重要的是 要告诉C++编译器 你要给我分配内存 ,我再继承类中 用到了a 不然会报错..

//3 A类中添加构造函数 
	//A类的构造函数中   A的构造函数是私有的构造函数 ... 
	//被别的类继承要小心....
	//单例场景 .... UML

void main801()
{
	A801 a1;
	a1.print();
	a1.SetA(999);
	B801 b1;
	b1.print();      //b1和a1用的是同一个static变量a
	system("pause");
}

void main802()
{
	B801  b1;
	//b1.a = 200; //error 不可访问
	system("pause");
}
