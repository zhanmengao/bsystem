#include<iostream>
#include <string>
using namespace std;
void getNext(const string &zi, int* next);
static int KMP(const string &fu, const string &zi, int pos = 0)
{
	int i = pos;
	int j = 0;
	//获取Next数组
	int nextArr[255] = { 0 };
	getNext(zi, nextArr);
	//开始匹配
	while (i < fu.length())
	{
		//比较
		if (j == 0 || fu[i] == zi[j])
		{
			//后移
			i++;
			j++;
			//如果此时匹配完了
			if (j == zi.length())
			{
				return i - j + 1;
			}
		}
		//若此时失配->退回前一个数的匹配有几个相等
		else if (j > 0)
		{
			j = nextArr[j];
		}
	}
	return -1;
}
//拿到子串的next数组——在j位置失配，则看前面有几个和它相等
//前面有0个相等则为0，有1个相等则为1...
void getNext(const string &zi,int* next)
{
	//前缀索引
	int b = 0;
	//后缀索引
	int cur = 1;
	while (cur < zi.length())
	{
		//打印一下数组
		for (int i = 0; i < zi.length(); i++)
		{
			printf("Before	next [%u] : %d \n", i, next[i]);
		}
		printf("\n");
		//如果相等
		if (b == 0 ||  zi[b] == zi[cur])
		{
			b++;
			cur++;
			next[cur] = b;
		}
		//不相等的情况
		else
		{
			//回朔：当前字符的前缀与后缀不连续，如果在此处发生P与T的不匹配
			b = next[b];
		}
		//打印一下数组
		for (int i = 0; i < zi.length(); i++)
		{
			printf("After	next [%u] : %d \n", i, next[i]);
		}
		printf("\n");
	}
}
void mainKMP()
{
	string fu = "abcababcaabcabx";
	string zi = "abcabx";
	cout << KMP(fu, zi) << endl;
}