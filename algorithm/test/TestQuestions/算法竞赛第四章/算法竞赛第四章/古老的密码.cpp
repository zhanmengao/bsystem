#include<iostream>
#include <string>
using namespace std;

void MySwap(int &a, int &b);
void quickSort(int *array, int lo, int hi);
bool AncientCipher(char *str1, char *str2,int length)
{
	int cnt1[26];
	int cnt2[26];
	//统计两个字符串中，每个字母出现次数
	for (int i = 0; i < length; i++)
	{
		char a = str1[i];
		char b = str2[i];
		cnt1[(a - 97)] += 1;
		cnt2[(b - 97)]++;
	}
	//将数组排序
	quickSort(cnt1, 0,25);
	quickSort(cnt2, 0, 25);
	for (int i = 0; i < 26; i++)
	{
		if (cnt1[i] != cnt2[i])
		{
			return false;
		}
	}
	return true;
}
int Partition(int *array, int lo, int hi);
void quickSort(int *array, int lo, int hi)
{
	//划分
	if (lo < hi)
	{
		int key = Partition(array, lo, hi);
		quickSort(array, lo, key - 1);
		quickSort(array, key + 1, hi);
	}
}
int Partition(int *array, int lo, int hi)
{
	//选定关键数据
	int key = array[lo];
	int i = lo;
	for (int j = lo + 1; j <= hi; j++)
	{
		//如果发现比key小的数
		if (array[j] < key)
		{
			//交换j指向比key小的数，i指向大于等于key的数字 交换它们
			MySwap(array[++i], array[j]);
		}
	}
	//最后 交换key与array[i]
	MySwap(array[lo], array[i]);
	return i;
	
}
void MySwap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}
void mainPas()
{
	char *str1 = "woaica";
	char *str2 = "caiwos";
	if (AncientCipher(str1, str2, 5))
	{
		cout << "相等"<<endl;
	}
	else{
		cout << "不等"<<endl;
	}
}
