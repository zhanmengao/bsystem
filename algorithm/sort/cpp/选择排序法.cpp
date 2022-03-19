#include<iostream>
using namespace std;

template<class T>
void selectSort(T array[], int size)
{

	//外层循环：每次找出一个最小的
	for (int i = 0; i < size; i++)
	{
		//认为当前的数就是最小
		T min=array[i];
		int minIndex = i;
		for (int j = i + 1; j < size; j++)
		{
			if (min > array[j])
			{
				//让它做最小
				min = array[j];
				minIndex = j;
			}
		}
		//放到前面
		mySwap(array[minIndex], array[i]);
	}
}

template<class T>
void mySwap(T &obj1, T &obj2)
{
	T temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}
template<class T>
void print(T array[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}

