#include<iostream>
using namespace std;
template<class T>
void insertSort(T* array, int size)
{
	//�����һ������
	for (int i = 1; i < size; i++)
	{
		int j = i;
		//�����ǰһ����С ��һֱ��ǰ�ƶ�
		while (j > 0 && array[j] < array[j - 1])
		{
			mySwap(array[k], array[j]);
			j--;
			k--;
		}
	}
}


