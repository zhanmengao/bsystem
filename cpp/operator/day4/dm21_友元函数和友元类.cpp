
#include <iostream>
using namespace std;

class A
{
public:
	friend class B;//B类 是 A的好朋友 ，在B中可以访问A类的私有成员 私有函数
	//1 声明的位置 和 public private没有关系
	friend void modifyA(A *pA, int _a); //2 函数modifyA 是 类A的好朋友
	A(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	int getA()
	{
		return this->a;
	}

private:
	int a;
	int b;
};

//  

void modifyA(A *pA, int _a)
{
	//pA->a = 100;
	pA->a = _a;
}

//
class B
{
public:
	void Set(int a)
	{
		Aobject.a = a;
	}
	void printB()
	{
		cout<<Aobject.a <<endl;
	}
private:
	A Aobject;
};

//为什么设计友元类函数
// 1.java--->1.class（字节码） ==》反射机制分析1.class 找到类对象。直接修改类的私有属性。。。
//反射机制 成为一种标准。。。。jdk ...sun 做成标准 。。。jdk 的 api函数中有体现 
//AOP
//2 1.cpp===>汇编
// 预编译 编译  连接  生成 。。gcc -E //gcc -s  -
//gcc -o 1.exe 1.c 
// 汇编往会找。。。。很难。。。。
//3 开了一个后门 。。。friend

/*
gcc -E hello.c -o hello.i（预处理）
gcc -S hello.i -o hello.s（编译）
gcc -c hello.s -o hello.o（汇编）
gcc hello.o -o hello（链接）
以上四个步骤，可合成一个步骤

gcc hello.c -o hello（直接编译链接成可执行目标文件）
gcc -c hello.c或gcc -c hello.c -o hello.o（编译生成可重定位目标文件）
*/

 

void main2101()
{
	B b1;
	b1.Set(300);
	b1.printB();
	system("pause");
}
void main2102()
{
	
	A a1(1, 2);
	cout<< a1.getA()<<endl;
	modifyA(&a1, 300);
	cout<< a1.getA()<<endl;

	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}