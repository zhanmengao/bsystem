#pragma once
#include<stdlib.h>
#include<assert.h>

void mainmalloc()
{
	char *str = (char*)malloc(100);
	str = (char*)realloc(str, 50);				//更改以前分配的内存长度（增加或减少）。
	assert(str != nullptr);
	free(str);
	str = nullptr;

	//为指定长度的对象，分配能容纳其指定个数的内存。申请到的内存的每一位（bit）都初始化为 0。
	str = (char*)calloc(100, sizeof(char));		
	assert(str != nullptr);
	free(str);
	str = nullptr;

	int* t = new int();     // 先内存分配 ，再构造函数
	delete t;           // 先析构函数，再内存释放


}