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
	//�����㷨
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

	//�������򣺽�һ��ֵ���뵽�Ѿ��ź����������У��Ӷ��õ�һ���µģ�������+1�������
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
        //����һ��ԭ����
        for (int i = 0; i < arrSize; i++)
        {
            arr[i] = pArr[beginPos + i];
        }
        //���
        const unsigned end = endPos - beginPos;
        const unsigned mid = end / 2;
        unsigned pLeft = 0;				//�������ָ��
        unsigned pRight = mid + 1;						//�Ұ�����ָ��
        for (int i = 0; i < arrSize; i++)
        {
            unsigned pArrIndex = beginPos + i;
            //ǰ��������������ߵ���
            if (pLeft > mid)
            {
                pArr[pArrIndex] = arr[pRight++];
            }
            else if (pRight > end)
            {
                pArr[pArrIndex] = arr[pLeft++];
            }
            //������������Ƚϣ���С�ķ���
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
			//��
			unsigned mid = beginPos + (endPos - beginPos) / 2;
			MereSort<T>(pArr, beginPos, mid);
			MereSort<T>(pArr, mid + 1, endPos);
			//������ú����������Ϊ����
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
        //ѡ��Key
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
        //��key��һ������ʼ���ѱ�keyС����ǰ��
        for (unsigned i = beginPos + 1; i <= endPos; i++)
        {
            if (pArr[i] < keyValue)
            {
                Swap<T>(pArr[i], pArr[++key]);
            }
        }
        //��ǰkeyָ��ָ�����һ����keyС������beginPosָ��key
        //��key�ŵ��м䣬���һ����KeyС�����ŵ��ײ�
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
		//�����������������
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
