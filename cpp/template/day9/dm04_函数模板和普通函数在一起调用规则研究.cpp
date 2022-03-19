
/*
函数模板不允许自动类型转化
普通函数能够进行自动类型转换
*/

/*
	1 函数模板可以像普通函数一样被重载
	2 C++编译器优先考虑普通函数
	3 如果函数模板可以产生一个更好的匹配，那么选择模板
	4 可以通过空模板实参列表的语法限定编译器只通过模板匹配
	*/


#include "iostream"
using namespace std;

namespace demo4
{

	int Max(int a, int b)
	{
		cout << "int Max(int a, int b)" << endl;
		return a > b ? a : b;
	}

	template<typename T>
	T Max(T a, T b)
	{
		cout << "T Max(T a, T b)" << endl;
		return a > b ? a : b;
	}

	template<typename T>
	T Max(T a, T b, T c)
	{
		cout << "T Max(T a, T b, T c)" << endl;
		return Max(Max(a, b), c);
	}

}

using namespace demo4;
void main401()
{
	int a = 1;
	int b = 2;

	cout << Max(a, b) << endl; //当函数模板和普通函数都符合调用时,优先选择普通函数
	cout << Max<int>(a, b) << endl; //若显示使用函数模板,则使用<> 类型列表

	cout << Max(3.0, 4.0) << endl; //如果 函数模板产生更好的匹配 使用函数模板

	cout << Max(5.0, 6.0, 7.0) << endl; //模板函数重载

	cout << Max('a', 100) << endl;  //调用普通函数 可以隐式类型转换 
}

