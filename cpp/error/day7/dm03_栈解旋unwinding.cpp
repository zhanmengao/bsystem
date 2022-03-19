
#include <iostream>
using namespace std;

class Test3
{
public:
	Test3(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	~Test3()
	{
	}
private:
	int a;
	int b;
};

void myDivide() throw (int, char, char *)            //在函数声明后面加throw 表示这个函数可能抛出这些类型的异常
{
	Test3 t1(1, 2), t2(3, 4);
	//throw Test3;
	throw 1;
}


//只能是 所列出类型
void myDivide1() throw (int, char, char *)
{
	Test3 t1(1, 2), t2(3, 4);
	//throw Test3;  //error：只能抛出指定类型的异常
}

//不写 或throw(...),可以抛出任何类型
void myDivide2() 
{
	Test3 t1(1, 2), t2(3, 4);
	cout << "myDivide ...要发生异常\n" ;
	//throw Test3;
	throw 1;
}
// 不抛出异常
void myDivide3() throw ()
{
	Test3 t1(1, 2), t2(3, 4);
	cout << "myDivide ...要发生异常\n" ;
	//throw Test3;
	throw 1;
}


void main301()
{
	try
	{
		myDivide();
	}
	catch (int a)
	{
		cout << "int类型 异常\n" ;
	}

	catch (...)
	{
		cout << " 未知 异常\n" ;
	}
}