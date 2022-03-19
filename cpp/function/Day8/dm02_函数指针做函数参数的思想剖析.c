#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int myadd(int a, int b)  //子任务的实现者
{
	printf("func add() do...\n");
	return a + b;
}
int myadd2(int a, int b)  //子任务的实现者
{
	printf("func add2() do...\n");
	return a + b;
}
int myadd3(int a, int b)  //子任务的实现者
{
	printf("func add3() do...\n");
	return a + b;
}
int myadd4(int a, int b)  //子任务的实现者
{
	printf("func add4() do...\n");
	return a + b;
}


typedef int (*MyTypeFuncAdd)(int a, int b);     //定义了一个函数指针类型 

//函数指针 做 函数参数
int MainOp(MyTypeFuncAdd myFuncAdd)
{
	int c = myFuncAdd(5, 6);
	return c;
}

// int (*MyPointerFuncAdd)(int a, int b)
int MainOp2(int (*MyPointerFuncAdd)(int a, int b) )
{
	int c = MyPointerFuncAdd(5, 6); //间接调用
	return c;
}

//间接调用 

//任务的调用 和 任务的编写可以分开
void main02()
{
	myadd(1, 2); //直接调用

	MyTypeFuncAdd  myFuncAdd = myadd;
	myFuncAdd(3, 4); //间接调用 

	//等价
	MainOp2(myadd);
	MainOp(myadd);

	//在mainop框架 没有发生任何变化的情况下 ...
	MainOp(myadd2);
	MainOp(myadd3);
	MainOp(myadd4);
}