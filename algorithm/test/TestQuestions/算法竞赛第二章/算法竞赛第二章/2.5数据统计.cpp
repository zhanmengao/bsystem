#include<iostream>
#include <string>
using namespace std;

void tongji(int *array,int size,int *max,int *min,int *avg)
{
	*max = 0;
	*min = 100000;
	*avg = 0;
	for (int i = 0; i < size; i++)
	{
		if (array[i]>1000)
		{
			return;
		}
		if (array[i]<*min)
		{
			*min = array[i];
		}
		if (array[i] > *max)
		{
			*max = array[i];
		}
		*avg += array[i];
	}
	(*avg)/=size;
}
void main25555()
{
	int array[] = { 1, 33, 55, 99, 3, 8, 2, 2, 5 };
	int max, min, avg;

	tongji(array, 9, &max, &min, &avg);
	cout<<max<<","<<min<<","<<avg<<endl;

}
