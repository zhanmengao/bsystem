#include<iostream>
#include<string>
#include<vector>
#include <cctype>
using namespace std;

int main317()
{
	vector<string> v1;
	string s1;
	while (cin >> s1)
	{
		v1.push_back(s1);
	}
	
	//输出改变后的结果
	for (auto iter = v1.begin(); iter != v1.end();)
	{
		string s1 = *iter;
		cout << "原字符串"<<s1<<endl;
		for (int i = 0; i < s1.size();i++)
		{
			auto& a = s1.at(i);
			cout << toupper(a);
		}
		iter++;
	}
	return 0;
}
