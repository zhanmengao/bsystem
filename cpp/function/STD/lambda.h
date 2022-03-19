#pragma once
#include <iostream>
#include <functional>
using namespace std;

auto g_Minus = [](int i, int j) { return i - j; };

int mainlll()
{
    std::function<int(int, int)> f = g_Minus;
	cout << f(1, 2) << endl;                                            // -1
	return 1;
}
