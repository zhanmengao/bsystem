#include<iostream>
#include <string>
using namespace std;
int main33333()
{
	int size;
	cin >> size;
	if (size < 2 || size>100)
	{
		return 0;
	}
	int *daoda = new int[size];
	int *likai = new int[size];
	for (int i = 0; i < size; i++)
	{
		cin >> daoda[i];
	}
	fflush(stdin);
	for (int i = 0; i < size; i++)
	{
		cin >> likai[i];
		if (likai[i] < daoda[i])
		{
			return 0;
		}
	}
	int max = 0;
	for (int i = 0; i < size; i++)
	{
		int count = 1;
		//i与每一个进行检测
		for (int j = 0; j < size; j++)
		{
			if (daoda[i]>daoda[j] && daoda[i] < likai[j])
			{
				count++;
			}
		}
		if (count > max)
		{
			max = count;
		}
	}
	cout << max;
}
template<class T>
void quickSort(T *array, int lo, int hi)
{
	if (lo < hi)
	{
		//拿到关键值位置
		int posi = partition(array, lo, hi);
		//划分
		quickSort(array, lo, posi - 1);
		quickSort(array, posi + 1, hi);
	}
}
template<class T>
int partition(T *array, int lo, int hi)
{
	T key = array[lo];
	//i指向一个比key大的数
	int i = lo;
	for (int j = lo + 1; j <= hi; j++)
	{
		//发现小的，就往前换
		if (array[j] <= key)
		{
			i++;
			mySwap<int>(array[i], array[j]);
		}
	}
	mySwap<int>(array[lo], array[i]);
	return i;
}


void mySwap(int i,int j)
{
	
}
