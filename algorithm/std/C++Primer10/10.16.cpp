#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
using namespace std;

/*功能描述：拿到vector中长度大于lengthLimit的单词
参数：
返回值：*/
void print(const vector<string>& v1);
void Biggies(vector<string> &words, const vector<string>::size_type& lengthLimit)
{
	stable_sort(words.begin(), words.end(), [](const string& str1, const string& str2){return str1.length() < str2.length(); });	//按长度排序
	print(words);
	auto it = unique(words.begin(), words.end());     //把重复的放到后面，返回最后一个不重复元素之后的位置
	print(words);
	it = find_if(words.begin(), words.end(), [lengthLimit](const string& str){return str.length() >= lengthLimit; });
	cout << words.size() << endl;
	for (it; it != words.end();)
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;




}
void print(const vector<string>& v1)
{
	for (auto it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
int main16()
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
	Biggies(v1, 4);
	return 0;
}