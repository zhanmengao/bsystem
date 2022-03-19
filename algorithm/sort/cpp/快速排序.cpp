#include<iostream>
#include <string>
using namespace std;

template<class T>
void mySwap(T &obj1, T &obj2)
{
	T temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}
template<class T>
void print(T array[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}
template<class T>
int partition(T *array, int lo, int hi)
{
    //随机一个关键值
    T key = array[lo];
    //交换关键值到第一个位置
    //mySwap<int>(array[lo], array[lo + (hi - lo) / 2]);
    //i指向一个比key大的数
    int i = lo;
    for (int j = lo + 1; j <= hi; j++)
    {
        //发现小的，就往前换
        if (array[j] <= key)
        {
            i++;
            mySwap<int>(array[i], array[j]);
        }
    }
    mySwap<int>(array[lo], array[i]);
    return i;
}
template<class T>
void quickSort(T *array,int lo,int hi)
{
    if (lo < hi)
    {
        //拿到关键值位置
        int posi = partition(array, lo, hi);
        //划分
        quickSort(array, lo, posi-1);
        quickSort(array, posi+1, hi);
    }
}
int main()
{
	int array[] = { 1, 9, 3, 6, 4, 5 };
	print<int>(array, sizeof(array) / sizeof(int));
	quickSort<int>(array, 0, 5);
	print<int>(array, sizeof(array) / sizeof(int));
    return 0;
}
