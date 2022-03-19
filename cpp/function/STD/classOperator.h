#pragma once
#include <iostream>
#include <functional>
using namespace std;

struct Minus
{
	int operator() (int i, int j)
	{
		return i - j;
	}
};

int main1()
{
	function<int(int, int)> f = Minus();
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}

template <class T>
struct Minus2
{
	T operator() (T i, T j)
	{
		return i - j;
	}
};

int mainoper()
{
	function<int(int, int)> f = Minus2<int>();
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}