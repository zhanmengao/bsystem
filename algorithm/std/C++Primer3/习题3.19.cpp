#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main319()
{
	//定义含有10个元素的vector，每个都是42
	//思路1：v1(num,val);
	vector<int> v1(10,42);
	//思路2:
	vector<int> v2;
	for (int i = 0; i < 10; i++)
	{
		v2.push_back(42);
	}
	//思路3
	vector<int> v3 = { 42, 42, 42, 42, 42, 42, 42, 42, 42, 42};
	//思路4
	vector<int> v4{ 42, 42, 42, 42, 42, 42, 42, 42, 42, 42 };
	//思路5
	vector<int> v5(10);
	for (auto &val : v5)
	{
		val = 42;
	}

	return 0;
}