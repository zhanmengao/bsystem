#include<iostream>
#include<string>
#include<vector>
#include<cctype>
using namespace std;

int main310()
{
	string s1;
	getline(cin, s1);
	//遍历 去除标点
	for (const char &a : s1)
	{
		if (!ispunct(a))
		{
			cout << a;
		}
	}
	cout << endl;
	return 0;

}