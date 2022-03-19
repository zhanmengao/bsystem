#include"Sort.h"
#include"SortTools.h"
template<typename T>
int BubbleSort(T* arr,int size)
{
    for(int i = 0;i<size;i++)
    {
        for(int j = i+1;j<size;j++)
        {
            if(arr[i] > arr[j])
            {
                exchange(arr[i],arr[j]);
            }
        }
    }
    return 0;
}
