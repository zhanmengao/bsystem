#include<iostream>
#include <string>
using namespace std;
//蛮力法
int MaxChengji(int *array, int size)
{
	int max = 0;
	//找到开头
	for (int i = 0; i < size; i++)
	{
		//找到每一个结尾
		for (int j = i; j < size; j++)
		{
			int chengji = 1;
			//累乘：从i乘到j
			for (int k = i; k <= j; k++)
			{
				chengji *= array[k];
				if (chengji > max)
				{
					max = chengji;
				}
			}
		}
	}
	return max;
}
//蛮力法优化
int BetterMaxChengji(int *array, int size)
{
	//找到开头
	for (int i = 0; i < size; i++)
	{
		//找到结尾
		for (int j = i; j < size; j++)
		{
			int chengji = 1;

			
		}
	}
	return 0;
}
void main444()
{
	int array[] = { 2,5,-1,2,-1 };
	cout<<MaxChengji(array,5)<<endl;
}