
#include <iostream>
using namespace std;

class BB
{
public:
	void printC()
	{
		cout<<"c = "<<c<<endl;
	}
	void AddC()
	{
		c = c + 1;
	}
	static void getC() //静态成员函数 
	{
		cout<<"c:"<<c<<endl;
		//请在静态成员函数中,能调用 普通成员属性  或者 普通成员函数吗?
		//cout<<"a:"<<a<<endl; //error C2597: 对非静态成员“BB::a”的非法引用
	}
protected:
private:
	int a; 
	int b;
	static int c; //静态成员变量
};

//静态函数中 不能使用 普通成员变量 普通成员函数 ..
int BB::c = 10;

void main1501()
{
	
	BB b1, b2, b3;
	b1.printC(); //10
	b2.AddC(); //11
	b3.printC(); //11
	

	//静态成员函数的调用方法
	b3.getC(); //用对象.
	BB::getC();//类::

}