#pragma once
#include<functional>
#include<vector>
#include<algorithm>
#include<iostream>
#include<iterator>
#include<memory>
void mashHash()
{
	/*
	ʹ�� transform() �㷨����ϣ vector �е�Ԫ�ء�
	transform() �����е�ǰ����������ָ���˱�����Ԫ�صķ�Χ��
	������������һ��ָ�������ַ�ĵ�������������һ�� ostream ��������
	���һ��������Ӧ�õ���ΧԪ���ϵĺ������� hash<int>��
	*/
	std::hash<int> hash_int;// Function object to hash int
	std::vector<int> n{-5, -2, 2, 5, 10};
	std::transform(std::begin(n), std::end(n), std::ostream_iterator<size_t>(std::cout, " "), hash_int);
	std::cout << std::endl;

	std::hash<double> hash_double;
	std::vector<double> x{ 3.14,-2.71828, 99.0, 1.61803399,6.62606957E-34 };
	std::transform(std::begin(x), std::end(x), std::ostream_iterator<size_t>(std::cout, " "), hash_double);
	std::cout << std::endl;

	struct Box
	{
		int a;
		int b;
		int c;
	};
	
	{
		//��ָͨ��
		std::hash<Box*> hash_box; // Box class as in Chapter 2
		Box box{ 1, 2, 3 };
		// Hash value = 2916986638
		std::cout << "Hash value = " << hash_box(&box) << std::endl;
	}


	{
		//����ָ���ָ��
		std::hash<Box*> hash_box; // Box class as in Chapter 2
		auto upbox = std::make_unique<Box>(1, 2, 3);
		// Hash value = 1143026886
		std::cout << "Hash value = " << hash_box(upbox.get()) << std::endl;
	}

	
	{
		//����ָ�뱾��
		std::hash<std::unique_ptr<Box>>hash_box; // Box class as in Chapter 2
		auto upbox = std::make_unique<Box>(1, 2, 3);
		// Hash value = 4291053140
		std::cout << "Hash value = " << hash_box(upbox) << std::endl; 
	}
	
}