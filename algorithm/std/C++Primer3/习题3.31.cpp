#include<iostream>
#include<string>
#include<vector>
using namespace std;

void print(int *arr)
{
	for (int i = 0; i < 10; i++)
	{
		cout << *(arr + i) << " ";
	}
}
int main331()
{
	int arr[10];
	print(arr);//´ËÊ±ÊÇÂÒÂë
	for (int i = 0; i < 10; i++)
	{
		arr[i] = i;
	}
	print(arr);
	return 0;
}