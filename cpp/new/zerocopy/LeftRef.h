#pragma once
#include<stdio.h>

int LeftMain() {
	int a = 5;				//a����ֵ��������ȡ��ַ��ֵ
	printf("a addr = %02.x \n", &a);
	int &b = a;			//b����ֵ����
	return 0;
}