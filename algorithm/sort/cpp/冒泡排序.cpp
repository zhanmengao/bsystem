#include<iostream>
#include<string>
#include<vector>
#include "SortTools.h"
using namespace std;

typedef void(*myPTypeFunc)(int a, int b);

//��ν�ʷ���t1�Ƿ�����򰴽���
//��ν�ʷ���t1�Ƿ��С��������
template<class T>
void BubbleSort(vector<T> &v1,bool (*compare)(const T& t1, const T& t2))
{
	for (unsigned i = 0; i < v1.size(); i++)
	{
		for (unsigned j = i; j < v1.size(); j++)
		{
			//�������ν��������v1[i]����v1[j]��С�ڣ�
			if (compare(v1[i], v1[j]))
			{
				//�Ƶ���ǰ�棬��֤index=i��Զ�ǵ�ǰ�����С����
				exchange<T>(v1[i], v1[j]);
			}
		}
	}
}
