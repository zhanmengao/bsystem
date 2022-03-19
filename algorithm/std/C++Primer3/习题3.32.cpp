#include<iostream>
#include<string>
#include<vector>
#include<iterator>
using namespace std;

void print332(int *arr)
{
	for (int i = 0; i < 10; i++)
	{
		cout << *(arr + i) << " ";
	}
}
int main332()
{
	int arr[10];
	for (int i = 0; i < 10; i++)
	{
		arr[i] = i;
	}
	//¿½±´µ½Êý×éb
	int arr2[sizeof(arr) / sizeof(arr[0])];
	for (int i = 0; i < 10; i++)
	{
		arr2[i] = arr[i];
	}
	print332(arr2);
	return 0;
}