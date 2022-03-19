#include<iostream>
using namespace std;

template<class T>
void selectSort(T array[], int size)
{

	//���ѭ����ÿ���ҳ�һ����С��
	for (int i = 0; i < size; i++)
	{
		//��Ϊ��ǰ����������С
		T min=array[i];
		int minIndex = i;
		for (int j = i + 1; j < size; j++)
		{
			if (min > array[j])
			{
				//��������С
				min = array[j];
				minIndex = j;
			}
		}
		//�ŵ�ǰ��
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

