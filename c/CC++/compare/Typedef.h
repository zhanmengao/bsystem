#pragma once
#include<stdio.h>

typedef int(MyArrayType)[5];					//��������
typedef int(*pArr)[5];					//����ָ��->ָ��int [5]
typedef void(*Func)(int a, int b);			//����ָ��

static void TypedefMain()
{
	//����������������ͬ����Ϊ��������
	int arr[5] = { 1,2,3,4,5 };
	MyArrayType arr2[5] = { 1,3,4,5,5 };
	printf("arr value: %p		&arr value:%p \n", arr, &arr);
	printf("arr2 value: %p		&arr2 value:%p \n", arr2, &arr2);
	//����������������ͬ����Ϊ����ָ��
	int(*pArray)[5] = &arr;	
	pArr a = &arr;
	printf("pArray value: %p		a value:%p \n", *pArray, *a);
}
