#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class Async;
using AsyncCallBack = int(Async::*)(int a, int b);     //定义了一个函数指针类型 
class Async
{
public:
	Async()
	{
		//MainOp(myadd2);        //报错：函数调用缺少参数列表，请使用&Async:myadd2创建指向成员的指针 
		//MainOp(Async::myadd3);
		MainOp(&Async::myadd4);
	}
public:
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
//函数指针 做 函数参数
int MainOp(AsyncCallBack myFuncAdd)
{
	int c = (this->*myFuncAdd)(5, 6);
	return c;
}
protected:
private:
};

//间接调用 

//任务的调用 和 任务的编写可以分开
void main03()
{
	Async aaa;
}