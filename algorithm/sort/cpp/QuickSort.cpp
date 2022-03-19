#include"Sort.h"
#include"SortTools.h"

int GetPos(int* array,int hi,int lo)
{
    int key = hi;
    int kval = array[key];
    for(int i = hi+1;i<=lo;i++)
    {
        //make left is small
        if(array[i] < kval)
        {
            exchange(array[i],array[++key]);
        }
    }
    exchange(array[key],array[hi]);
    return key;
}
int QuickSort(int *array,int hi,int lo)
{
    if(hi>=lo)
    {
        return 0;
    }
    int pos = GetPos(array,hi,lo);
    QuickSort(array,hi,pos);
    QuickSort(array,pos+1,lo);
}
