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
	
	//����ı��Ľ��
	for (auto iter = v1.begin(); iter != v1.end();)
	{
		string s1 = *iter;
		cout << "ԭ�ַ���"<<s1<<endl;
		for (int i = 0; i < s1.size();i++)
		{
			auto& a = s1.at(i);
			cout << toupper(a);
		}
		iter++;
	}
	return 0;
}
