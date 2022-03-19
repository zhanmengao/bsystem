#include"Sort.h"
#include<stdint.h>
#include<limits.h>
#include"SortTools.h"
int PartFind(int *arr,int hi,int lo,int val)
{
    if(hi >= lo)
    {
        return -1;
    }
    int mid = (lo - hi) /2 + hi;
    if(arr[mid] == val)
    {
        return mid;
    }
    else if(arr[mid] >val)
    {
        return PartFind(arr,hi,mid - 1,val);
    }
    else
    {
        return PartFind(arr,mid+1,lo,val);
    }
}

template<typename T>
int GetSecondMaxIndex(T *arr, int n)
{
    int maxIndex = 0;
    T max = arr[maxIndex];
    int secondIndex = 1;
    T secondMax = arr[secondIndex];
    if(max < secondMax)
    {
        exchange<T>(max,secondMax);
        exchange<int>(maxIndex,secondIndex);
    }
    for(int i = 2 ; i < n ; ++i)
    {
        if(arr[i] > max)
        {
            secondMax = max;
            max = arr[i];
        }
        else if(arr[i] > secondMax)
        {
            secondMax = arr[i];
        }
    }
    return secondIndex;
}
