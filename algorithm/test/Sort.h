#pragma once
#include<stdio.h>
#define SIZE(arr) sizeof(arr) / sizeof(int)
static void Swap(int* a, int* b)
{
	int c = *a;
	*a = *b;
	*b = c;
}
static void Print(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("arr[%d] = %d \n", i, arr[i]);
	}
}
static int BubbleSort(int *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (arr[i] < arr[j])
			{
				Swap(arr + i, arr + j);
			}
		}
	}
	return 0;
}

static int SelectSort(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		int val = arr[i];
		int index = i;
		for (int j = i + 1; j < size; j++)
		{
			if (val < arr[j])
			{
				val = arr[j];
				index = j;
			}
		}
		Swap(arr + i, arr + index);
	}
	return 0;
}

//快速排序,说白了就是给基准数据找其正确索引位置的过程.
static int GetKey(int *arr, int begin, int end)
{
	if (begin == end)
	{
		return begin;
	}
	int key = arr[begin];
	int index = begin + 1;
	for (int i = index; i <= end; i++)
	{
		if (arr[i] > key)
		{
			Swap(arr + i, arr + index);
			index++;
		}
	}
	Swap(arr + begin, arr + index - 1);
	return index;
}
static int QuickSort(int* arr, int begin, int end)
{
	if (end > begin)
	{
		int pos = GetKey(arr, begin, end);
		QuickSort(arr, begin, pos -1);
		QuickSort(arr, pos + 1, end);
	}
	return 0;
}

void mainSort()
{
	{
		int arr[] = { 3,2,1,4,5 };
		BubbleSort(arr, SIZE(arr));
		Print(arr, SIZE(arr));
	}
	{
		int arr[] = { 3,2,1,4,5 };
		SelectSort(arr, SIZE(arr));
		Print(arr, SIZE(arr));
	}
	{
		int arr[] = { 3,2,1,4,5 };
		QuickSort(arr, 0,SIZE(arr)-1);
		Print(arr, SIZE(arr));
	}
}