#include<iostream>
#include <string>
using namespace std;

//方法1 计数排序法
//返回所有数出现的频率
int* countSort(int *array, int lo, int hi, int size)
{
	if (hi < lo)
	{
		throw 1;
	}
	//遍历输入，找到所有数出现频率
	int *count = new int[hi - lo + 1];
	for (int i = 0; i < size; i++)
	{
		count[array[i]-lo]++;
	}
	return count;
}
//不修改数组找出重复数字->二分查找思想



