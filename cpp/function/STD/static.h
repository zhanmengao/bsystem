#pragma once
#include <iostream>
#include <functional>
using namespace std;

class Math
{
public:
	static int Minus(int i, int j)
	{
		return i - j;
	}
};

int mainss()
{
    std::function<int(int, int)> f = &Math::Minus;
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}
