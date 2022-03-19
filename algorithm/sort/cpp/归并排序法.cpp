#include<iostream>
#include <string>
using namespace std;
template<class T>
void margeSort(T *array, int size)
{
	margeSort(array, 0, size-1);
}

template<class T>
//划分
void margeSort(T *array, int lo, int hi)
{
	if (lo >= hi)
	{
		return;
	}
	else{
		//划分
		//左边
		//cout << "lo=" << lo << ", hi=" << hi << endl;
		margeSort(array, lo,lo+(hi-lo)/2);
		//右边
		margeSort(array, lo + (hi - lo) / 2 + 1, hi);
		//归并（本次递归的lo和hi）
		marge(array, lo, hi);
	}
}
//归并
template<class T>
void marge(T *array,int lo,int hi)
{
	//拷贝一个新数组
	T *arr = new T[hi - lo+1];
	for (int i = 0; i < hi-lo+1; i++)
	{
		//索引位置0存放array[lo],依次类推
		*(arr+i) = *(array+i+lo);
	}
	//临时数组的索引位
	int mid = (hi - lo) / 2;
	//cout <<"lo="<<lo<<" hi="<<hi <<" mid="<<mid<<endl;
	int headIndex = 0;//前半索引
	int midIndex = mid + 1;//后半索引(mid算前半)
	//归并(从lo到hi)
	for (int i = lo; i < hi; i++)
	{
		//临界条件检查
	
		//前半到达边界
		if (headIndex > mid)
		{
			//直接放右边索引
			*(array+i) = arr[midIndex++];
		}
		//后半到达边界
		else if (midIndex >hi-lo+1)
		{
			//直接放左边索引
			*(array+i) = arr[headIndex++];
		}
		//左边的小
		else if (arr[headIndex]<arr[midIndex])
		{
			*(array+i) = arr[headIndex++];
		}
		//右边的小
		else{
			*(array + i) = arr[midIndex++];
		}
	}
	//cout<<endl;
	delete[] arr;
}

