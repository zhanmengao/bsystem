#include <vector>
#include <iostream> 
using namespace std;
/*
模板偏特化（Template Partitial Specialization）是模板特化的一种特殊情况，
指显示指定部分模板参数而非全部模板参数，或者指定模板参数的部分特性分而非全部特性，也称为模板部分特化。
与模板偏特化相对的是模板全特化，指对所有的模板参数进行特化。模板全特化与模板偏特化共同组成模板特化。

模板偏特化主要分为两种，一种是指对部分模板参数进行全特化，另一种是对模板参数特性进行特化，包括将模板参数特化为指针、引用或是另外一个模板类。
*/
//函数模板
template<typename T, class N> void compare(T num1, N num2)
{
	cout << "standard function template" << endl;
	if (num1 > num2)
		cout << "num1:" << num1 << " > num2:" << num2 << endl;
	else
		cout << "num1:" << num1 << " <= num2:" << num2 << endl;
}

//对部分模板参数进行特化
template<class N> void compare(int num1, N num2)
{
	cout << "partitial specialization" << endl;
	if (num1 > num2)
		cout << "num1:" << num1 << " > num2:" << num2 << endl;
	else
		cout << "num1:" << num1 << " <= num2:" << num2 << endl;
}

//将模板参数特化为指针
template<typename T, class N> void compare(T* num1, N* num2)
{
	cout << "new partitial specialization" << endl;
	if (*num1 > *num2)
		cout << "num1:" << *num1 << " > num2:" << *num2 << endl;
	else
		cout << "num1:" << *num1 << " <= num2:" << *num2 << endl;
}

//将模板参数特化为另一个模板类
template<typename T, class N> void compare(std::vector<T>& vecLeft, std::vector<T>& vecRight)
{
	cout << "to vector partitial specialization" << endl;
	if (vecLeft.size() > vecRight.size())
		cout << "vecLeft.size()" << vecLeft.size() << " > vecRight.size():" << vecRight.size() << endl;
	else
		cout << "vecLeft.size()" << vecLeft.size() << " <= vecRight.size():" << vecRight.size() << endl;
}

void mainFunc()
{
	compare<int, int>(30, 31);//调用非特化版本compare<int,int>(int num1, int num2)

	compare(30, '1');		//调用偏特化版本compare<char>(int num1, char num2)

	int a = 30;
	char c = '1';
	compare(&a, &c);		//调用偏特化版本compare<int,char>(int* num1, char* num2)

	vector<int> vecLeft{ 0 };
	vector<int> vecRight{ 1, 2, 3 };
	compare<int, int>(vecLeft, vecRight);	//调用偏特化版本compare<int,char>(int* num1, char* num2)
}
