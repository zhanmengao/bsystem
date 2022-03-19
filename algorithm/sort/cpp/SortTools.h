#include<iostream>
#include<string>
#include<vector>
#include <iterator>
using namespace std;

#define sz(arr) sizeof(arr)/sizeof(int)
template<class T>
inline void exchange(T& t1,T& t2)
{
	T temp = t1;
	t1 = t2;
	t2 = temp;
}

inline void Swap(int& a,int& b)
{
    b = b^a;            //
    a = a^b;            //x = (x^x)^y == y
    b = a^b;            // y = (y^y)^x == x
}

inline void Swap(int* a, int* b)
{
    *a = *a^*b;
    *b = *b^*a;
    *a = *b^ *a;
}
template<class T>
inline bool isSort(vector<T> arr,bool(*compare)(const T& t1, const T& t2))
{
	for (int i = 0; i < arr.size() - 1; i++)
	{
		if (compare(arr[i],arr[i+1]))
		{
			return false;
		}
	}
	return true;
}

inline bool isSort(int* arr,int size)
{
    for (int i = 0; i < size- 1; i++)
    {
        if (arr[i] > arr[i+1])
        {
            return false;
        }
    }
    return true;
}

template<class T>
inline void print(vector<T> arr)
{
	for (auto iter = arr.begin(); iter != arr.end();)
	{
		cout << *iter << " ";
		iter++;
	}
	cout <<endl;
}

inline void print(int* arr,unsigned length)
{
	for (unsigned i = 0; i < length; i++)
	{
		cout << arr[i] << " ";
	}
	cout<<endl;
	
}
