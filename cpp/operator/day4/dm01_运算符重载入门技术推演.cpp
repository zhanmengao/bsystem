
#include <iostream>
using namespace std;

class Complex
{
public:
	int a;
	int b;
public:
	Complex(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	void printCom()
	{
		cout<<a<<" + " << b << "i" <<endl;
	}
};

//1 定义了全局函数
Complex myAdd(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a + c2.a, c1.b+ c2.b);
	return tmp; //
}

//2 函数名 升级
Complex operator+(Complex &c1, Complex &c2)
{
	cout<<"12345上山 打老虎"<<endl;
	Complex tmp(c1.a + c2.a, c1.b+ c2.b);
	return tmp; //
}

Complex operator/(Complex &c1, Complex &c2)
{
	cout << "12345上山 打老虎" << endl;
	Complex tmp(c1.a / c2.a, c1.b / c2.b);
	return tmp; //
}

void main101()
{
	
	int a = 0, b = 0;
	int c;
	c = a + b; //1 基础数据类型 编译器已经知道了. 如何运算

	// a + bi 复数运算规则
	Complex c1(1, 2), c2(3, 4);
	Complex c3; //2 类 也是一种数据类型  用户自定义数据类型 C++编译器 是不知道如何进行运算
	//c3 = c1 + c2 ;
	//c1--; --c1

	//3 c++编译器应该给我们程序员提供一种机制 ... 
	//让自定义数据类型 有机会 进行 运算符操作 ====> 运算符重载机制 

	//4 运算符重载机制

	//步骤1 
	//Complex c4 = myAdd(c1, c2);
	//c4.printCom();

	//步骤2  //Complex c4  = c1 + c2
	//Complex c4 = operator+(c1, c2);
	//c4.printCom();

	//步骤3 
	Complex c4 = c1 + c2;
	c4.printCom();
	
	//步骤3 
	Complex c4 = c1 / c2;
	c4.printCom();

	//总结: 1 运算符重载的本质 是 函数调用 



	cout<<"hello..."<<endl;
	system("pause");
	return ;
}