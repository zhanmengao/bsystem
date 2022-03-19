#include<iostream>
#include<string>
#include<vector>
#include "SortTools.h"
using namespace std;

template<class T>
void SelectSort(vector<T> &arr, bool(*compare)(const T& t1, const T& t2))
{
	for (unsigned i = 0; i < arr.size(); i++)
	{
		//��ǰ��ֵ
		T top = 0;
		//��ֵ����
		unsigned topIndex = 0;
		for (unsigned j = i; j < arr.size(); j++)
		{
			//���������ֵɸѡ����
			//���磺���arr[j]>top�򷵻�true
			if (compare(top,arr[j]))
			{
				top = arr[j];
				topIndex = j;
			}
		}
		//��Ŀ��ֵ�Ƶ���ǰ��
		exchange(arr[i], arr[topIndex]);
	}
}