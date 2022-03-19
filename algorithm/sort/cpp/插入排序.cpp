#include<iostream>
#include<string>
#include<vector>
#include "SortTools.h"
using namespace std;

template<class T>
void InsertSort(vector<T> &arr, bool(*compare)(const T& t1, const T& t2))
{
	if (arr.empty())
	{
		return;
	}
	//����ǰ��i-1�����������
	for (unsigned i = 1; i < arr.size(); i++)
	{
		//��ǰ����
		int j = i;
		while (j > 0)
		{
			//��ǰһ�����Ƚϣ���arr[j]��arr[j-1]���򷵻�true
			if (compare(arr[j - 1], arr[j]))
			{
				exchange(arr[j - 1], arr[j]);
				j--;
			}
			//������Ϊ�ҵ�����ȷ��λ��
			else 
			{
				break;
			}
		}
	}
}