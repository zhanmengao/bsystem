#pragma once
#include<stdio.h>
int main()
{
	char str1[] = "Hello World";					//这是字符数组
	char str2[] = "Hello World";

	char *str3 = "Hello World";					//这是字面量，指向常量区，指向同一个地址
	char *str4 = "Hello World";	

	//比较地址
	printf("str1 addr: %p \n", str1);
	printf("str2 addr: %p \n", str2);
	printf("str3 addr: %p \n", str3);
	printf("str4 addr: %p \n", str4);
	if (str1 == str2)
	{
		printf("str1 and str2 are same\n");
	}
	else
	{
		printf("str1 and str2 are not same\n");
	}
	if (str3 == str4)
	{
		printf("str3 and str4 are same\n");
	}
	else
	{
		printf("str3 and str4 are not same\n");
	}
	return 0;
}