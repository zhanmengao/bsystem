#include<iostream>
#include <string>
#include <vector>
using namespace std;

//替换空格
void ReplaceBlank(string &str, const string& regex, const string &replacement)
{
	//遍历找到所有regex
}
int* getNext(const string& zi);
int KMP(const string &fu, const string& zi,int pos=0)
{
	if (pos >= fu.length())
	{
		return -1;
	}
	//获取Next数组
	int *next = getNext(zi);
	//父串索引
	int i = pos;
	//子串索引
	int j = 0;
	//返回从pos开始第一个匹配的信息
	while (i<fu.length())
	{
		if (fu[i] == zi[j])
		{
			i++;
			j++;
			if (j >= zi.length())
			{
				return i - j;
			}
		}
		//不匹配->j回朔
		else if(j>0){
			j = next[j - 1];
		}
		else{
			i++;
		}
	}
	return -1;
}
int* getNext(const string& zi)
{
	int *next = new int[zi.length()];
	//前缀没有相等则为0，有一个相等则为1
	next[0] = 0;
	//前缀
	int top = 0;
	//后缀
	int bottom = 1;
	//遍历zi
	while (top < zi.length())
	{
		if (zi[top]==zi[bottom])
		{
			next[top] = bottom + 1;
			//后移继续比较
			top++;
			bottom++;
		}
		else if(top>0){
			//回朔
			bottom = next[bottom - 1];
		}
		else{
			next[bottom] = 0;
			top++;
		}
	}
	return next;
}
void main255()
{
	vector<int> v1;
	cout << sizeof(v1)<<endl;
}