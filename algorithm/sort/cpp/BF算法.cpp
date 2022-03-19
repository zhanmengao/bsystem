#include<iostream>
#include <string>
using namespace std;

//返回在第几位遇到
int BF(const string &zhu, const string &zi,int pos=0)
{
	//主串当前位置
	int i = pos;
	//子串当前位置
	int j = 0;
	//i小于主串长度时退出循环，返回-1
	while (i < zhu.length())
	{
		if (zhu[i] == zi[j])
		{
			//后移
			i++;
			j++;
			//如果此时匹配完了
			if (j == zi.length())
			{
				return i-j+1;
			}
		}
		else
		{
			//退回本次开始匹配的地方后一位
			i = i - j + 1;
			j = 0;
		}
	}
	return -1;
}
