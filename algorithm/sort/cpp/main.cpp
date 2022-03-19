#include <iostream>
#include"Sort.h"
#include"SortTools.h"
using namespace std;

int main()
{
    int arr[] = {9,2,4,6,11,7,5};
    QuickSort(arr,0,sz(arr) - 1);
    print(arr,sz(arr));

    cout<<PartFind(arr,0,sz(arr),9)<<endl;
    return 0;
}
