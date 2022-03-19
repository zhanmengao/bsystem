#include<iostream>
#include <string>
using namespace std;
float select(float *array, int lo, int hi, int pos);
void main333()
{
	int size;
	cin >> size;
	float *array = new float[size];
	for (int i = 0; i < size; i++)
	{
		cin >> *(array + i);
	}
	//找到第二大的数->等价于找到第size-1小
	cout<<select(array, 0, size - 1, size-1)<<endl;
	delete [] array;
}
template<class T>
int partition(T *array, int lo, int hi)
{
	//取关键值
	T key = array[lo];
	//i指向一个比key大的数
	int i = lo;
	for (int j = lo + 1; j <= hi; j++)
	{
		//发现小的，就往前换
		if (array[j] <= key)
		{
			i++;
			mySwap<float>(array[i], array[j]);
		}
	}
	mySwap<float>(array[lo], array[i]);
	return i;
}
template<class T>
void mySwap(T &obj1, T &obj2)
{
	T temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}
float select(float *array, int lo, int hi, int pos)
{
	//基本情况
	if (lo == hi)
		return array[lo];
	//拿到key在数组中的下标
	int q = partition<float>(array, lo, hi);
	//k为当前小数组中，array[q]的元素的个数
	int k = q - lo + 1;
	//如果幸运地k为第q个元素
	if (pos == k)
	{
		return array[q];
	}
	else if(pos<k){
		//找左边
		return select(array, lo, q - 1, pos);
	}
	else{
		return select(array, q + 1, hi, pos-k);
	}
}

