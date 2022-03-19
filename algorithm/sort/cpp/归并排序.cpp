#include<iostream>
#include<string>
#include<vector>
using namespace std;

template<class T>
void MereSort(T *array, int lo, int hi)
{
	if (lo >= hi)
	{
		return;
	}
	unsigned mid = lo + (hi - lo) / 2;
	//先分
	MereSort<T>(array, lo, mid);
	MereSort<T>(array, mid + 1, hi);
	//再合
	Mere<T>(array, lo, hi);
}
template<class T>
//将局部有序的数组合并
void Mere(T *array, const int& lo, const int& hi)
{
	T *temp = new T[hi - lo + 1];
	//拷贝
	for (int i = 0; i < hi - lo + 1; i++)
	{
		temp[i] = array[lo + i];
	}
	//往array的[lo,hi]定义域放数据
	const int midIndex = (hi - lo) / 2;
	const int endIndex = hi - lo;
	int l= 0;                         //左下标
	int r = midIndex + 1;              //右下标
	for (int i = lo; i <= hi; i++)
	{
		if (l > midIndex)
		{
			//放右边的
			array[i] = temp[r++];
		}
		else if (r > endIndex)
		{
			//放左边的
			array[i] = temp[l++];
		}
		else if (temp[l] <= temp[r])
		{
			array[i] = temp[l++];
		}
		else{
			array[i] = temp[r++];
		}
	}
	delete[] temp;
}