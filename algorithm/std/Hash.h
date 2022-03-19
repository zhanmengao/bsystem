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
	使用 transform() 算法来哈希 vector 中的元素。
	transform() 参数中的前两个迭代器指定了被操作元素的范围，
	第三个参数是一个指定输出地址的迭代器，这里是一个 ostream 迭代器，
	最后一个参数是应用到范围元素上的函数对象 hash<int>。
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
		//普通指针
		std::hash<Box*> hash_box; // Box class as in Chapter 2
		Box box{ 1, 2, 3 };
		// Hash value = 2916986638
		std::cout << "Hash value = " << hash_box(&box) << std::endl;
	}


	{
		//智能指针的指针
		std::hash<Box*> hash_box; // Box class as in Chapter 2
		auto upbox = std::make_unique<Box>(1, 2, 3);
		// Hash value = 1143026886
		std::cout << "Hash value = " << hash_box(upbox.get()) << std::endl;
	}

	
	{
		//智能指针本身
		std::hash<std::unique_ptr<Box>>hash_box; // Box class as in Chapter 2
		auto upbox = std::make_unique<Box>(1, 2, 3);
		// Hash value = 4291053140
		std::cout << "Hash value = " << hash_box(upbox) << std::endl; 
	}
	
}