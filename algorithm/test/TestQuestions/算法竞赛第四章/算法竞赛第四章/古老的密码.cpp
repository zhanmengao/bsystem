#include<iostream>
#include <string>
using namespace std;

void MySwap(int &a, int &b);
void quickSort(int *array, int lo, int hi);
bool AncientCipher(char *str1, char *str2,int length)
{
	int cnt1[26];
	int cnt2[26];
	//ͳ�������ַ����У�ÿ����ĸ���ִ���
	for (int i = 0; i < length; i++)
	{
		char a = str1[i];
		char b = str2[i];
		cnt1[(a - 97)] += 1;
		cnt2[(b - 97)]++;
	}
	//����������
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
	//����
	if (lo < hi)
	{
		int key = Partition(array, lo, hi);
		quickSort(array, lo, key - 1);
		quickSort(array, key + 1, hi);
	}
}
int Partition(int *array, int lo, int hi)
{
	//ѡ���ؼ�����
	int key = array[lo];
	int i = lo;
	for (int j = lo + 1; j <= hi; j++)
	{
		//������ֱ�keyС����
		if (array[j] < key)
		{
			//����jָ���keyС������iָ����ڵ���key������ ��������
			MySwap(array[++i], array[j]);
		}
	}
	//��� ����key��array[i]
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
		cout << "���"<<endl;
	}
	else{
		cout << "����"<<endl;
	}
}
