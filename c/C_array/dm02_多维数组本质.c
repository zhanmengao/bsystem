#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void printArray01(int a[3][5]) //4k
{
	int i, j, tmp = 0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", a[i][j]);
		}
	}
}

void printArray02(int a[][5]) //5k
{
	int i, j, tmp = 0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", a[i][j]);
		}
	}
}

void printArray03( int (*b)[5]) //6k
{
	int i, j, tmp = 0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", b[i][j]);
		}
	}
}

//多维数组做函数参数 退化过程。。。。。。事实存在。。。
//为什么 
void main()
{
	int a[3][5], i=0, j=0;
	int tmp = 1;

	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			a[i][j] = tmp++;
		}
	}

	printf("----------------\n");
	printArray03(a);

	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", a[i][j]);
		}
	}

	// a 多维数组名 代表 ？
	printf("a %d , a+1:%d ", a, a+1);  //a+1的步长 是20个字节 5*4
	printf("&a %d , &a+1:%d ", &a, &a+1);

	{
		//定义一个指向数组的指针变量 
		int (*pArray)[5] ;//告诉编译器  分配 4个字节的内存 32bit平台下
		pArray = a;

		printf("\n");
		for (i=0; i<3; i++)
		{
			for (j=0; j<5; j++)
			{
				printf("%d ", pArray[i][j]);
			}
		}
	}
	printf("\n");

	//多维数组名的本质  数组指针 ，步长 一维的长度

	//
	// (a+i) 代表是整个第i行的地址  二级指针
	// *(a+i) 代表 1级指针  第i行首元素的地址
	// *(a+i) + j  ===> &  a[i][j]

	//*( *(a+i) + j) ===>a[i][j]元素的值


	//a[i][j] <=== >*( *(a+i) + j)

	//a[i] ===> a[0+i] ==> *(a+i);

	//a[i][j] === a[0+i][j]  ==>  *(a+i)[j]  ===>  *(a+i)[0 + j] ==>  *( *(a+i)  + j) 
	const char* str[] = { "aa","bbb","cccc" };
	const char** pstr;
	printf("str %d  pstr = %d\n", sizeof(str),sizeof(pstr));
}