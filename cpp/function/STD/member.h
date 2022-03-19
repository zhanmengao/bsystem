#pragma once
#include <iostream>
#include <functional>
using namespace std;

class Math
{
public:
	int Minus(int i, int j)
	{
		return i - j;
	}
};

int mainmmm()
{
	Math m;
    function<int(int, int)> f = std::bind(&Math::Minus, &m, placeholders::_1, placeholders::_2);
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}


class Math2
{
public:
	template <class T>
	T Minus(T i, T j)
	{
		return i - j;
	}
};

int main3123123()
{
	Math2 m;
    function<int(int, int)> f = std::bind(&Math2::Minus<int>, &m, placeholders::_1, placeholders::_2);
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}
