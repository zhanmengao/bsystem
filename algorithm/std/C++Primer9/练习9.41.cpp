#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main941()
{
	vector<char> v1;
	v1.push_back('a');
	v1.push_back('a');
	v1.push_back('a');
	v1.push_back('a');
	v1.push_back('a');
	string s(v1.begin(), v1.end());
	cout<<s<<endl;
	return 0;
}
