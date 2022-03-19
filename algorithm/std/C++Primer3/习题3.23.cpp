#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main323()
{
	vector<int> v1(10, 42);
	for (auto iter = v1.begin(); iter != v1.end();)
	{
		(*iter) *= 2;
		iter++;
	}
	for (auto iter = v1.begin(); iter != v1.end();)
	{
		cout << *iter << "  ";
		iter++;
	}
	return 0;
}