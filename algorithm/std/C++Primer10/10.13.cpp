#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;
void print(const vector<string>& v1);
inline bool IsLengthOut5(const string& str)
{
	return str.length() >= 4;
}
void  PartitionVector(vector<string> &words)
{
	partition(words.begin(),words.end(), IsLengthOut5);
	print(words);
}
int main13()
{
	vector<string> v1;
	v1.push_back("aaa");
	v1.push_back("aaa");
	v1.push_back("aaa");
	v1.push_back("bbb");
	v1.push_back("bbb");
	v1.push_back("aaa");
	v1.push_back("ccc");
	v1.push_back("aeeaa");
	v1.push_back("eee");
	v1.push_back("qqq");
	v1.push_back("sss");
	PartitionVector(v1);
   return 0;
}