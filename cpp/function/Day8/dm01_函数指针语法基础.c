
#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//数组指针 语法 梳理
//定义一个数组类型   
//int a[10];//

//定义一个指针数组类型 

//定义一个指向 数组类型的指针 数组类的指针

void main101()
{
	int a[10]; //a代表的是数组首元素的地址  &a代表整个数组的地址  a+1 4  &a+1步长 40 .

	{
		//定义一个数组类型         --->定义的是int[10]类型的别名
		typedef int (myTypeArray)[10]; //
		myTypeArray myArray;
		myArray[0] = 10;
		printf("%d \n", myArray[0]);
	}

	{
		//定义一个指针数组类型    ----->定义的是指向(int[10])类型指针的别名
		typedef int (*PTypeArray)[10];  //int *p 

		PTypeArray myPArray; //sizeof(int) *10
		myPArray = &a;
		//int b = 10;
		//int *p = NULL;
		//p = &b;
		(*myPArray)[0] = 20;

		printf("a[0]: %d \n", a[0]);
	
	}

	{
		//定义一个指向 数组类型的指针 数组类的指针   ----->定义的是指针变量

		int (*MyPointer)[10]; //变量 告诉C编译器 给我分配内存
		MyPointer = &a;
		(*MyPointer)[0] = 40;
		printf("a[0]: %d \n", a[0]);
	}

}



//函数指针语法梳理
//1 如何定义一个函数类型
//2 如何定义一个函数指针类型
//3 如何定义一个 函数指针  (指向一个函数的入口地址)

int add(int a, int b)
{
	printf("func add ....\n");
	return a +b;
}

void main102()
{
	add(1, 2); //直接调用调用 //函数名就是函数的入口地址 

	//定义一个函数类型
	{
		typedef int (MyFuncType)(int a, int b); //定义了一个类型
		MyFuncType *myPointerFunc = NULL; //定义了一个指针, 指向某一种类的函数..

		myPointerFunc  = &add;  //细节
		myPointerFunc(3, 4); //间接调用

		myPointerFunc  = add;  //细节 //C 过程 兼容历史版本的原因
		myPointerFunc(3, 4); //间接调用
	}

	//定义一个函数指针类型
	{
		typedef int (*MyPointerFuncType)(int a, int b); //int * a = NULL;
		MyPointerFuncType myPonterFunc; //定义一个指针
		myPonterFunc = add;
		myPonterFunc(5, 6);
	}

	//函数指针 
	{
		int (*MyPonterFunc)(int a, int b); //定义了一个变量
		MyPonterFunc = add;
		MyPonterFunc(7, 8);
	}
}

