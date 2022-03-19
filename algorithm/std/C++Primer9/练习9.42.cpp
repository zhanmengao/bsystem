#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main942()
{
	string s;
	vector<char> v1;
	//每次读一个字符
	char c;
	while (cin>>c)
	{
		v1.push_back(c);
		if (v1.size() == 100)
		{
			s.append(v1.begin(), v1.end());
			v1.clear();
		}
	}
	cout<<s<<endl;
	return 0;
}
