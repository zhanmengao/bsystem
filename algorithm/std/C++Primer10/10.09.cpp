#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
using namespace std;

void print(const vector<string>& v1);
/*功能描述：unique 重排并去重
参数：
返回值：*/
void elimDups(vector<string> &v1)
{
	stable_sort(v1.begin(), v1.end());
	print(v1);
	auto it=unique(v1.begin(), v1.end());
	print(v1);
}

int main09()
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
	elimDups(v1);
   return 0;
}