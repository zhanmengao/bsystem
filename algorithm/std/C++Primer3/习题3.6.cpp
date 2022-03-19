#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main306()
{
	string s1;
	getline(cin, s1);
	cout<<s1<<endl;
	for (auto &s : s1)
	{
		s = 'X';
	}
	cout << s1 << endl;
	return 0;
}
