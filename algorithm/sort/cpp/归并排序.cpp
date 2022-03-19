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
	//�ȷ�
	MereSort<T>(array, lo, mid);
	MereSort<T>(array, mid + 1, hi);
	//�ٺ�
	Mere<T>(array, lo, hi);
}
template<class T>
//���ֲ����������ϲ�
void Mere(T *array, const int& lo, const int& hi)
{
	T *temp = new T[hi - lo + 1];
	//����
	for (int i = 0; i < hi - lo + 1; i++)
	{
		temp[i] = array[lo + i];
	}
	//��array��[lo,hi]�����������
	const int midIndex = (hi - lo) / 2;
	const int endIndex = hi - lo;
	int l= 0;                         //���±�
	int r = midIndex + 1;              //���±�
	for (int i = lo; i <= hi; i++)
	{
		if (l > midIndex)
		{
			//���ұߵ�
			array[i] = temp[r++];
		}
		else if (r > endIndex)
		{
			//����ߵ�
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