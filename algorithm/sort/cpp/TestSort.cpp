#include<iostream>
#include<string>
#include<vector>
//#include "ð������.cpp"
//#include"ѡ������.cpp"
#include "��������.cpp"
#include "�鲢����.cpp"
using namespace std;

//�����ν�ʣ���b��a���бȽϣ���b��a���򷵻�true
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