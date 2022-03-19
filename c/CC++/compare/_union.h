#pragma once
#include<iostream>

union UnionTest 
{
	//构造函数
	UnionTest() : i(10) 
	{
	};
	int i;
	double d;
};

static union
{
	int i;
	double d;
};

int mainUnion() 
{
	UnionTest u;

	//匿名联合体
	union 
	{
		int i;
		double d;
	};

	std::cout << u.i << std::endl;  // 输出 UnionTest 联合的 10

	::i = 20;
	std::cout << ::i << std::endl;  // 输出全局静态匿名联合的 20

	i = 30;
	std::cout << i << std::endl;    // 输出局部匿名联合的 30

	return 0;
}