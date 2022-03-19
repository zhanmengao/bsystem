#include<iostream>
#include<vector>
#include<string>
using namespace std;

void Sum()
{
	int ii[] = { 2, 35, 342, 3123, 22, 335, 552, 223, 44 };
	
	
}

int main14()
{
   [](const int& a, const int& b)->int{ return a + b; };         //完全体：[捕获列表] (参数列表)->返回类型{ 函数体 }
   [](const int& a, const int& b){ return a + b; };             //省略返回类型
   []{return 9; };                                              //商略参数列表和返回类型
   return 0;
}