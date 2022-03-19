#pragma once
#include"../Queue/Queue.h"
#include"../Queue/Stack.h"
#include<ctime>
namespace stb
{
	void Swap(int& a, int &b)
	{
		b = a^b;
		a = a^b;
		b = a^b;
	}
	void Revert(int* arr, unsigned nSize)
	{
		unsigned first = 0;
		unsigned last = nSize - 1;
		for (; first < last; first++, last--)
		{
			Swap(arr[first], arr[last]);
		}
	}
	bool compare(int a, int b)
	{
		return a^b != 0;
	}
	template<class T>
	void Swap(T& l, T& r)
	{
		T tmp = l;
		l = r;
		r = tmp;
	}
	int Random(int min, int max)
	{
		srand(time(NULL));
		return min + rand() % (max - min);
	}
	//排序算法
	template<class T>
	static void BubbleSort(T *arr, unsigned size)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if (arr[i] > arr[j])
				{
					Swap<int>(arr[i], arr[j]);
				}
			}
		}
	}

	//插入排序：将一个值插入到已经排好序的有序表中，从而得到一个新的，排序数+1的有序表
	template<class T>
	static void InsertSort(T* arr, unsigned size)
	{
		for (int i = 1; i < size; i++)
		{
			int count = i;
			while (count > 0 && arr[count] < arr[count - 1])
			{
				Swap<T>(arr[count], arr[count - 1]);
				count--;
			}
		}
	}

	template<class T>
	static void PickSort(T *arr, unsigned size)
	{
		T minVal;
		unsigned minIndex;
		for (int i = 0; i < size; i++)
		{
			minVal = arr[i];
			minIndex = i;
			for (int j = i + 1; j < size; j++)
			{
				if (minVal > arr[j])
				{
					minVal = arr[j];
					minIndex = j;
				}
			}
			Swap<T>(arr[i], arr[minIndex]);
		}
	}
	namespace mere
	{
    template<class T>
    static void Mere(T* pArr, unsigned beginPos, unsigned endPos)
    {
        if (beginPos >= endPos)
        {
            return;
        }
        const unsigned arrSize = endPos - beginPos + 1;
        int* arr = new T[arrSize];
        //拷贝一个原数组
        for (int i = 0; i < arrSize; i++)
        {
            arr[i] = pArr[beginPos + i];
        }
        //填充
        const unsigned end = endPos - beginPos;
        const unsigned mid = end / 2;
        unsigned pLeft = 0;				//左半数组指针
        unsigned pRight = mid + 1;						//右半数组指针
        for (int i = 0; i < arrSize; i++)
        {
            unsigned pArrIndex = beginPos + i;
            //前两种情况，左右走到底
            if (pLeft > mid)
            {
                pArr[pArrIndex] = arr[pRight++];
            }
            else if (pRight > end)
            {
                pArr[pArrIndex] = arr[pLeft++];
            }
            //后两种情况，比较，把小的放入
            else if (arr[pLeft] < arr[pRight])
            {
                pArr[pArrIndex] = arr[pLeft++];
            }
            else
            {
                pArr[pArrIndex] = arr[pRight++];
            }
        }
        delete[]arr;
    }
		template<class T>
        static void MereSort(T* pArr, unsigned beginPos, unsigned endPos)
		{
			if (beginPos >= endPos)
			{
				return;
			}
			//分
			unsigned mid = beginPos + (endPos - beginPos) / 2;
			MereSort<T>(pArr, beginPos, mid);
			MereSort<T>(pArr, mid + 1, endPos);
			//上面调用后，左右数组均为有序
			Mere<T>(pArr, beginPos, endPos);
#if __DEBUG
			for (int i = beginPos; i <= endPos; i++)
			{
				printf("mere end arr[%u] = %d\n", i, pArr[i]);
			}
			printf("\n");
#endif
		}

	}
	template<class T>
	static void MereSort(T *pArr, unsigned size)
	{
		mere::MereSort<T>(pArr, 0, size - 1);
	}

	namespace quick
	{
    template<class T>
    static unsigned GetKey(T* pArr, unsigned beginPos, unsigned endPos)
    {
        //选定Key
        unsigned key = Random(beginPos, endPos);
        Swap<T>(pArr[key], pArr[beginPos]);

        key = beginPos;
        T keyValue = pArr[key];
#if __DEBUG
        printf("key value:%d\n", keyValue);
        for (int i = beginPos; i <= endPos; i++)
        {
            printf("GetKey Begin arr[%u] = %d\n", i, pArr[i]);
        }
#endif
        //从key后一个数开始，把比key小的往前放
        for (unsigned i = beginPos + 1; i <= endPos; i++)
        {
            if (pArr[i] < keyValue)
            {
                Swap<T>(pArr[i], pArr[++key]);
            }
        }
        //当前key指针指向最后一个比key小的数，beginPos指向key
        //把key放到中间，最后一个比Key小的数放到首部
        Swap<T>(pArr[beginPos], pArr[key]);

#if __DEBUG
        for (int i = beginPos; i <= endPos; i++)
        {
            printf("GetKey end arr[%u] = %d\n", i, pArr[i]);
        }
        printf("\n");
#endif
        return key;
    }
		template<class T>
		static void QuickSort(T* pArr, unsigned beginPos, unsigned endPos)
		{
			if (beginPos >= endPos)
			{
				return;
			}
			unsigned key = GetKey<T>(pArr, beginPos, endPos);
			QuickSort<T>(pArr, beginPos, key);
			QuickSort<T>(pArr, key + 1, endPos);
		}

	}
	template<class T>
	static void QuickSort(T *pArr, unsigned size)
	{
		quick::QuickSort<T>(pArr, 0, size - 1);
	}

	namespace Search
	{
		//输入升序排序的数组
		template<class T>
		static int BinarySearch(T* arr, unsigned beginPos, unsigned endPos, const  T& value)
		{
			unsigned mid = beginPos + (endPos - beginPos) / 2;
			if (arr[mid] == value)
			{
				return mid;
			}
			else if (beginPos >= endPos)
			{
				return -1;
			}
			else if (arr[mid] > value)
			{
				return BinarySearch(arr, beginPos, mid - 1, value);
			}
			else
			{
				return BinarySearch(arr, mid + 1, endPos, value);
			}
		}
	}

	void mainSort()
	{
		Vector<int> arr;
		for (int i = 0; i < 10; i++)
		{
			arr.PushBack(10 - i);
		}
		//BubbleSort<int>(arr.data(),arr.Size());
		//InsertSort<int>(arr.data(), arr.Size());
		//PickSort<int>(arr.data(), arr.Size());
		//MereSort(arr.data(), arr.Size());
		QuickSort(arr.data(), arr.Size());
		for (int i = 0; i < arr.Size(); i++)
		{
			printf("%d \n", arr[i]);
		}
	}
}
