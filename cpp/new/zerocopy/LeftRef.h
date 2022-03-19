#pragma once
#include<stdio.h>

int LeftMain() {
	int a = 5;				//a是左值——可以取地址的值
	printf("a addr = %02.x \n", &a);
	int &b = a;			//b是左值引用
	return 0;
}