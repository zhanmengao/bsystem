#pragma once
#include<vector>
#include<iostream>

std::vector<int> GetPrimeNumber(int n)
{
	std::vector<int> prime;
	if (n < 2)
	{
		return prime;
	}
	prime.push_back(2);
	for (int i = 3; i < n; i++)
	{
		bool bPrime = true;
		for (auto it = prime.begin(); it != prime.end(); it++)
		{
			//如果整除了素数
			if (i % *it == 0)
			{
				bPrime = false;
				break;
			}
		}
		if (bPrime)
		{
			prime.push_back(i);
		}
	}
	return prime;
}

void PrimeNumberMain()
{
	auto vec = GetPrimeNumber(100);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}