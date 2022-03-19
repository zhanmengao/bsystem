/*
ʹ�ú���ģ�����һ����������ĺ��� sort()��Ҫ����������ݴ���������� T a[n]
ģ�����TҪ֧���ַ�����
*/
#include <iostream>
#include <string>
using namespace std;

template<typename T>
void Sort(T* a, int n)
{
	int i, j;
	T t;
	for (i = 0; i < n - 1; i++)
	for (j = 0; j<n - i - 1; j++)
	if (a[j]>a[j + 1])
	{
		t = a[j];
		a[j] = a[j + 1];
		a[j + 1] = t;
	}
}

template<typename T>
void Print(T* a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		cout << a[i] << " ";
	cout << endl;
}

int main(int argc, char **argv)
{
	string Str[10] = { "Zhang", "Li", "Wang", "Qian", "Zhao", "Wu", "Xu", "Tang", "Shen", "Liang" };
	int Int[8] = { 20, 12, 0, -5, 9, -18, 6, 11 };
	double Dou[7] = { 3.07, 8.12, -0.45, 6, 10, -9, 7.29 };
	Sort<string>(Str, 10);
	Sort<int>(Int, 8);
	Sort<double>(Dou, 7);
	Print(Str, 10);
	Print(Int, 8);
	Print(Dou, 7);
	system("pause");
	return 0;
}