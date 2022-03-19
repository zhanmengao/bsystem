#include<iostream>
#include<string>
#include<vector>
#include "SortTools.h"
using namespace std;

typedef void(*myPTypeFunc)(int a, int b);

//若谓词返回t1是否更大，则按降序
//若谓词返回t1是否更小，则按升序
template<class T>
void BubbleSort(vector<T> &v1,bool (*compare)(const T& t1, const T& t2))
{
	for (unsigned i = 0; i < v1.size(); i++)
	{
		for (unsigned j = i; j < v1.size(); j++)
		{
			//如果满足谓词条件（v1[i]大于v1[j]或小于）
			if (compare(v1[i], v1[j]))
			{
				//移到最前面，保证index=i永远是当前最大（最小）的
				exchange<T>(v1[i], v1[j]);
			}
		}
	}
}
