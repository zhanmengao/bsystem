#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void getLen(int *p)
{
		*p = 30; //*谁的地址  就间接修改谁的值 
}

void main01()
{
	int a = 10;
	int *p = NULL;

	a = 11;

	p = &a; 
	*p = 20;

	{
		*p = 30; //*谁的地址  就间接修改谁的值 
	}
}


void getMem1101(char **p2 /*out*/)
{
	*p2  = 30; //间接赋值  p2是p的地址 

	*p2  = (char **)malloc(1000);
}

void main22()
{
	char *p = NULL;

	char **p2 = NULL;

	p = 1;
	p = 2;  //直接修改p的值

	p2 = &p;
	*p2  = 20;

	{
		*p2  = 30; //间接赋值  p2是p的地址 
	}
}



int getMem1102(char ***p3)
{
	*p3 = 100; //直接修改二级指针的值
}

void main03()
{

	char **p = NULL;

	char ***p3 = NULL;

	p = 1;
	p = 2;

	p3  = &p;

	*p3 = 10; //直接修改二级指针的值

	{
		*p3 = 30; //直接修改二级指针的值
	}

	getMem1102(&p);
}

int getMem1104(char ********p8)
{
		*p8 = 100; //* 的是 p7 的地址 所以 间接的修改了p7的值
}

void main311()
{
	char *******p7 = NULL;

	char ********p8 = NULL;

	p7 = 1;  //直接修改
	p7 = 2;

	p8 = &p7;

	*p8 = 10;

	{
		*p8 = 20; //* 的是 p7 的地址 所以 间接的修改了p7的值
	}
	getMem1104(&p7);

	printf("p7:%d \n", p7);
}



