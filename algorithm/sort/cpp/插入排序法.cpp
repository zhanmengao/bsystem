#include<iostream>
using namespace std;
template<class T>
void insertSort(T* array, int size)
{
	//假设第一个有序
	for (int i = 1; i < size; i++)
	{
		int j = i;
		//如果比前一个数小 则一直往前移动
		while (j > 0 && array[j] < array[j - 1])
		{
			mySwap(array[k], array[j]);
			j--;
			k--;
		}
	}
}


