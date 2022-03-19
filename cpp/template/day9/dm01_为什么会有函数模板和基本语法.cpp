
#include <iostream>
using namespace std;

namespace demo1
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


	//让 类型参数化 ===, 方便程序员进行编码
	// 泛型编程 
	//template 告诉C++编译器 我要开始泛型编程了 .看到T, 不要随便报错
	template <typename T>
	void myswap(T &a, T &b)
	{
		T c = 0;
		c = a;
		a = b;
		b = c;
		cout << "hello ....我是模板函数 欢迎 calll 我" << endl;
	}


	//函数模板的调用
	// 显示类型 调用
	// 自动类型 推导
	void main101()
	{
		{

			int x = 10;
			int y = 20;

			myswap<int>(x, y); //1 函数模板 显示类型 调用

			myswap(x, y);  //2 自动类型 推导
			printf("x:%d y:%d \n", x, y);
		}


	{

		char a = 'a';
		char b = 'b';

		//myswap<char>(a, b); //1 函数模板 显示类型 调用

		myswap(a, b);
		printf("a:%c b:%c \n", a, b);
	}
	}
}
using namespace demo1;
void main102()
{
	{
		int x = 10;
		int y = 20;

		myswap01(x, y);
		printf("x:%d y:%d \n", x, y);

	}

	{
		char a = 'a';
		char b = 'b';

		myswap02(a, b);
		printf("a:%c b:%c \n", a, b);
	};
}

