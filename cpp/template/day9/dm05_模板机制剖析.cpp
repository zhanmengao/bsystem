
#include <iostream>
using namespace std;

/*

函数模板机制结论
编译器并不是把函数模板处理成能够处理任意类的函数
编译器从函数模板通过具体类型产生不同的函数
编译器会对函数模板进行两次编译
在声明的地方对模板代码本身进行编译；在调用的地方对参数替换后的代码进行编译。
*/
namespace demo05
{
	// 函数的业务逻辑 一样
	// 函数的参数类型 不一样
	void myswap01(int &a, int &b)
	{
		int c = 0;
		c = a;
		a = b;
		b = c;
	}

	void myswap02(char &a, char &b)
	{
		char c = 0;
		c = a;
		a = b;
		b = c;
	}


	// 1.cpp

	// g++ -S 1.cpp  -o 1.s
	template <typename T>
	void myswap(T &a, T &b)
	{
		T c = NULL;
		c = a;
		a = b;
		b = c;
		cout << "hello ....我是模板函数 欢迎 calll 我" << endl;
	}
}

using namespace demo05;
void main501()
{
	{
		int x = 10;
		int y = 20;

		myswap<int>(x, y); //1 函数模板 显示类型 调用

		printf("x:%d y:%d \n", x, y);
	}


	{
		char a = 'a';
		char b = 'b';

		myswap<char>(a, b); //1 函数模板 显示类型 调用
		printf("a:%c b:%c \n", a, b);
	}
}
