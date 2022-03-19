#include<iostream>
#include<string>
#include<vector>
#include "SortTools.h"
using namespace std;

template<class T>
void InsertSort(vector<T> &arr, bool(*compare)(const T& t1, const T& t2))
{
	if (arr.empty())
	{
		return;
	}
	//假设前面i-1个数是有序的
	for (unsigned i = 1; i < arr.size(); i++)
	{
		//当前坐标
		int j = i;
		while (j > 0)
		{
			//和前一个数比较，若arr[j]比arr[j-1]大则返回true
			if (compare(arr[j - 1], arr[j]))
			{
				exchange(arr[j - 1], arr[j]);
				j--;
			}
			//否则认为找到了正确的位置
			else 
			{
				break;
			}
		}
	}
}