#pragma once
#include<iostream>
#include<string>
class ArrayItem
{
public:
	ArrayItem()
	{
		std::cout << "ArrayItem" << std::endl;
	}
};

int main()
{
	ArrayItem arr[10];

	const char* carr[] = { "aa","bbb","cccc" };
	return 0;
}
