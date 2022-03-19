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
	//�ҵ��ڶ������->�ȼ����ҵ���size-1С
	cout<<select(array, 0, size - 1, size-1)<<endl;
	delete [] array;
}
template<class T>
int partition(T *array, int lo, int hi)
{
	//ȡ�ؼ�ֵ
	T key = array[lo];
	//iָ��һ����key�����
	int i = lo;
	for (int j = lo + 1; j <= hi; j++)
	{
		//����С�ģ�����ǰ��
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
	//�������
	if (lo == hi)
		return array[lo];
	//�õ�key�������е��±�
	int q = partition<float>(array, lo, hi);
	//kΪ��ǰС�����У�array[q]��Ԫ�صĸ���
	int k = q - lo + 1;
	//������˵�kΪ��q��Ԫ��
	if (pos == k)
	{
		return array[q];
	}
	else if(pos<k){
		//�����
		return select(array, lo, q - 1, pos);
	}
	else{
		return select(array, q + 1, hi, pos-k);
	}
}

