#include<iostream>
#include <string>
using namespace std;

//����1 ��������
//�������������ֵ�Ƶ��
int* countSort(int *array, int lo, int hi, int size)
{
	if (hi < lo)
	{
		throw 1;
	}
	//�������룬�ҵ�����������Ƶ��
	int *count = new int[hi - lo + 1];
	for (int i = 0; i < size; i++)
	{
		count[array[i]-lo]++;
	}
	return count;
}
//���޸������ҳ��ظ�����->���ֲ���˼��



