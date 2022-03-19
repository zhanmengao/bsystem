#include<iostream>
#include <string>
using namespace std;
/*
动态规划核心思想 ： 动态规划是把一个大问题拆解成一堆小问题。
一个大问题之所以能用”动态规划“解决，并不是因为它能拆解成一堆小问题，事实上啥大问题都能拆解成小问题。
取决于该问题是否能用动态规划解决的是这些”小问题“会不会被被重复调用。
*/
long long *arr;
int jiecheng(int n)
{
	if (n > 1e6)
	{
		return 0;
	}
	//初始化
	arr = new long long[n+1];
	arr[0] = 1;
	arr[1] = 1;
	for (int i = 2; i < n; i++)
	{
		arr[i] = 0;
	}
	//计算和
	long long total = 0;
	//自底向上
	for (int i = 2; i <= n; i++)
	{
		arr[i] = i*arr[i - 1];
	}
	for (int i = 0; i <= n; i++)
	{
		cout <<i<<"!="<< arr[i]<<endl;
		total += arr[i];\
	}
	return total;
}

int jiecheng2(int n)
{
	//如果值存在
	if (arr[n] != 0)
	{
		return arr[n];
	}
	//如果不存在 就算出来
	else
	{
		arr[n] = n*jiecheng2(n-1);
	}
	return arr[n];
}

void main2444()
{
	int n = 0;
	cin >> n;
	cout<<n<<"的阶乘之和为"<<jiecheng(n)<<endl;
}

