
#include <iostream>
using namespace std;

//C语言中的变量都必须在作用域开始的位置定义！！
//C++中更强调语言的“实用性”，所有的变量都可以在需要使用时再定义。

void main401()
{
	int i;

	printf("hello...\n");

	int k;

	i = 10;
	k = 11;
	printf("i:%d k:%d \n", i, k);
	system("pause");

}

void main402()
{
	register int a = 0; 
	printf("&a: %d \n", &a); //不能在寄存器变量上取地址

	for (int i=0; i<1000; i++) //不使用register也可能做优化
	{
		printf("i:%d \n", i);
	}
	system("pause");
}



/*
在C语言中，重复定义多个同名的全局变量是合法的
	在C++中，不允许定义多个同名的全局变量
C语言中多个同名的全局变量最终会被链接到全局数据区的同一个地址空间上
int g_var;
int g_var = 1;

C++直接拒绝这种二义性的做法。
*/

int g_a = 100;
//int g_a ;

void main403()
{
	printf("hello...g_a:%d \n", g_a);
}



