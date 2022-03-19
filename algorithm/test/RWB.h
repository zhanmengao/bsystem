#pragma once
#include"Sort.h"
int RWB(int* arr, int size)
{
	int rc = 0;
	int wc = 0;
	int bc = 0;
	//¼ÆÊý
	for (int i = 0; i < size; i++)
	{
		if (arr[i] == 0)
		{
			rc++;
		}
		else if (arr[i] == 1)
		{
			wc++;
		}
		else if (arr[i] == 2)
		{
			bc++;
		}
	}
	//ÅÅÐò
	for (int i = 0; i < size; i++)
	{
		if (i < rc)
		{
			arr[i] = 0;
		}
		else if (i < rc + wc)
		{
			arr[i] = 1;
		}
		else
		{
			arr[i] = 2;
		}
	}
	return 0;
}

void RWBmain()
{
	int arr[] = { 0,1,0,2,2,1,0,2,1 };
	RWB(arr, SIZE(arr));
	Print(arr, SIZE(arr));
}