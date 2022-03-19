#pragma once
#include <iostream>
#include <functional>
using namespace std;

template <class T>
T g_Minus(T i, T j)
{
	return i - j;
}

int mainTemplate()
{
	function<int(int, int)> f = g_Minus<int>;
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}