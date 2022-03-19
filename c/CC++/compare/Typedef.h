#pragma once
#include<stdio.h>

typedef int(MyArrayType)[5];					//数组类型
typedef int(*pArr)[5];					//数组指针->指向int [5]
typedef void(*Func)(int a, int b);			//函数指针

static void TypedefMain()
{
	//这两个变量类型相同，均为数组类型
	int arr[5] = { 1,2,3,4,5 };
	MyArrayType arr2[5] = { 1,3,4,5,5 };
	printf("arr value: %p		&arr value:%p \n", arr, &arr);
	printf("arr2 value: %p		&arr2 value:%p \n", arr2, &arr2);
	//这两个变量类型相同，均为数组指针
	int(*pArray)[5] = &arr;	
	pArr a = &arr;
	printf("pArray value: %p		a value:%p \n", *pArray, *a);
}
