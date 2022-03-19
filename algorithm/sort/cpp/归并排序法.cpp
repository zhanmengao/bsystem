#include<iostream>
#include <string>
using namespace std;
template<class T>
void margeSort(T *array, int size)
{
	margeSort(array, 0, size-1);
}

template<class T>
//����
void margeSort(T *array, int lo, int hi)
{
	if (lo >= hi)
	{
		return;
	}
	else{
		//����
		//���
		//cout << "lo=" << lo << ", hi=" << hi << endl;
		margeSort(array, lo,lo+(hi-lo)/2);
		//�ұ�
		margeSort(array, lo + (hi - lo) / 2 + 1, hi);
		//�鲢�����εݹ��lo��hi��
		marge(array, lo, hi);
	}
}
//�鲢
template<class T>
void marge(T *array,int lo,int hi)
{
	//����һ��������
	T *arr = new T[hi - lo+1];
	for (int i = 0; i < hi-lo+1; i++)
	{
		//����λ��0���array[lo],��������
		*(arr+i) = *(array+i+lo);
	}
	//��ʱ���������λ
	int mid = (hi - lo) / 2;
	//cout <<"lo="<<lo<<" hi="<<hi <<" mid="<<mid<<endl;
	int headIndex = 0;//ǰ������
	int midIndex = mid + 1;//�������(mid��ǰ��)
	//�鲢(��lo��hi)
	for (int i = lo; i < hi; i++)
	{
		//�ٽ��������
	
		//ǰ�뵽��߽�
		if (headIndex > mid)
		{
			//ֱ�ӷ��ұ�����
			*(array+i) = arr[midIndex++];
		}
		//��뵽��߽�
		else if (midIndex >hi-lo+1)
		{
			//ֱ�ӷ��������
			*(array+i) = arr[headIndex++];
		}
		//��ߵ�С
		else if (arr[headIndex]<arr[midIndex])
		{
			*(array+i) = arr[headIndex++];
		}
		//�ұߵ�С
		else{
			*(array + i) = arr[midIndex++];
		}
	}
	//cout<<endl;
	delete[] arr;
}

