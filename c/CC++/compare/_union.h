#pragma once
#include<iostream>

union UnionTest 
{
	//���캯��
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

	//����������
	union 
	{
		int i;
		double d;
	};

	std::cout << u.i << std::endl;  // ��� UnionTest ���ϵ� 10

	::i = 20;
	std::cout << ::i << std::endl;  // ���ȫ�־�̬�������ϵ� 20

	i = 30;
	std::cout << i << std::endl;    // ����ֲ��������ϵ� 30

	return 0;
}