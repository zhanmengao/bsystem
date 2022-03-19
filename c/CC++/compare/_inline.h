#pragma once
#include<iostream>
using namespace std;
// 声明1（加 inline，建议使用）
inline int functionName(int first, int second, ...);

// 声明2（不加 inline）
int functionName(int first, int second, ...);

// 定义
inline int functionName(int first, int second, ...) {/****/ };

// 类内定义，隐式内联
class A 
{
	int doA() { return 0; }         // 隐式内联
};

// 类外定义，需要显式内联
class A 
{
	int doA();
};
inline int A::doA() { return 0; }   // 需要显式内联

//虚函数可以是内联函数，但具体执行看操作系统
class Base
{
public:
	inline virtual void who()
	{
		cout << "I am Base\n";
	}
	virtual ~Base() {}
};
class Derived : public Base
{
public:
	inline void who()  // 不写inline时隐式内联
	{
		cout << "I am Derived\n";
	}
};

int mainInlineVirtual()
{
	// 此处的虚函数 who()，是通过类（Base）的具体对象（b）来调用的，编译期间就能确定了，所以它可以是内联的，但最终是否内联取决于编译器。
	Base b;
	b.who();

	// 此处的虚函数是通过指针调用的，呈现多态性，需要在运行时期间才能确定，所以不能为内联。
	Base *ptr = new Derived();
	ptr->who();

	// 因为Base有虚析构函数（virtual ~Base() {}），所以 delete 时，会先调用派生类（Derived）析构函数，再调用基类（Base）析构函数，防止内存泄漏。
	delete ptr;
	ptr = nullptr;

	system("pause");
	return 0;
}