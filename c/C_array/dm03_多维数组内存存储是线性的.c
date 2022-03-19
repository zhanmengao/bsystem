#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void printfArray01(int *array, int size)
{
	int  i = 0;
	for (i=0; i<size; i++)
	{
		printf("%d ", array[i]);
	}
}
void main443()
{
	int a[3][5];
	int i, j, tmp = 1;

	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			a[i][j] = tmp++;
		}
	}

	//把二维数组 当成  1维数组 来打印 证明线性存储
	printfArray01((int *)a, 15);
}