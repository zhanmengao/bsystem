#include<iostream>
using namespace std;

template<class T>
void bubbling(T array[],int size)
{
	//���ѭ�� ÿ������һ��������С�ŵ���ǰ�棩
	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			
			if (array[i]>array[j])
			{
				//�ŵ���ǰ
				mySwap(array[i], array[j]);
			}		
		}
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
void print(T array[],int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << array[i]<<" ";
	}
	cout << endl;
}



