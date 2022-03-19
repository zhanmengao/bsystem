#include<iostream>
#include <string>
using namespace std;

void main777()
{
	int *array=new int[15];
	int index = 0;
	for (int i = 100; i <= 999; i++)
	{
		if (i == (i / 100)*(i / 100)*(i / 100) + (i / 10 % 10)*(i / 10 % 10)*(i / 10 % 10) + (i % 10)*(i % 10)*(i % 10))
		{
			array[index++] = i;
		}
	}
	for (int i = 0; i < 15; i++)
	{
		cout<<array[i]<<endl;
	}
	delete[] array;
}
