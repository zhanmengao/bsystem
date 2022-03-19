#include<iostream>
#include<string>
#include<vector>
//#include "冒泡排序.cpp"
//#include"选择排序.cpp"
#include "插入排序.cpp"
#include "归并排序.cpp"
using namespace std;

//降序的谓词：将b与a进行比较，若b比a大则返回true
bool Compare(const int& a, const int& b)
{
	if (b >= a)
	{
		return true;
	}
	else{
		return false;
	}
}

int mainSort()
{
	vector<int> v1 = { 22, 33, 4, 9, 123, 666, 2, 1, 0 };
	int arrar[9] = { 22, 33, 4, 9, 123, 666, 2, 1, 0 };
	//BubbleSort<int>(v1, Compare);
	//SelectSort<int>(v1, Compare);
	//InsertSort(v1, Compare);
	MereSort<int>(arrar, 0, sizeof(arrar) / sizeof(arrar[0]));
	print(arrar,9);
	return 0;
}