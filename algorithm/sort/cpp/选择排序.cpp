#include<iostream>
#include<string>
#include<vector>
#include "SortTools.h"
using namespace std;

template<class T>
void SelectSort(vector<T> &arr, bool(*compare)(const T& t1, const T& t2))
{
	for (unsigned i = 0; i < arr.size(); i++)
	{
		//当前最值
		T top = 0;
		//最值索引
		unsigned topIndex = 0;
		for (unsigned j = i; j < arr.size(); j++)
		{
			//如果满足最值筛选条件
			//例如：如果arr[j]>top则返回true
			if (compare(top,arr[j]))
			{
				top = arr[j];
				topIndex = j;
			}
		}
		//把目标值移到最前面
		exchange(arr[i], arr[topIndex]);
	}
}