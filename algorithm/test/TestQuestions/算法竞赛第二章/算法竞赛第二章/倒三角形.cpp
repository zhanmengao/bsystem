#include<iostream>
#include <string>
using namespace std;

void printSanjiao(int n)
{
	//外层循环：打印一层
	for (int i = n; i > 0; i--)
	{
		//打印空格
		//for (int j = n; j > ;j--)
		//打印#
		for (int j = 0; j < 2 * i - 1; j++)
		{
			cout << "#";
		}
		cout<<endl;
	}
}
void main22()
{
	printSanjiao(5);
}