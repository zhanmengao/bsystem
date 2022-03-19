#pragma once
#include<list>
#include<map>
#include<vector>
using namespace std;

class Solution
{
public:
	std::vector<int> getLeastNumbers(vector<int>& arr, int k)
	{
		std::vector<int> ret;
		for (auto it = arr.begin(); it != arr.end(); it++)
		{
			mArr[*it]++;
		}
		for (int i = 0; i<sizeof(arr) / sizeof(int); i++)
		{
			for (int j = 0; j<mArr[i]; j++)
			{
				ret.push_back(i);
			}
		}
		return ret;
	}
	vector<int> GetLeastNumbers(vector<int>& arr, int k)
	{
		if (arr.empty() || k == 0)
		{ 
			return{}; 
		}
		return quickSelection(arr, 0, arr.size() - 1, k - 1);								// 注意第 k 个数对应的下标是 k - 1
	}
	vector<int> quickSelection(vector<int>& arr, int left, int right, int index)
	{
		// partition函数将一个区间内所有小于下标为 j 的数放在 j 左边，大于下标为 j 的数放在 j 右边
		int j = partition(arr, left, right);

		if (j == index)
		{ 
			// 若 j 刚好等于 k - 1，将 arr[0] 至 arr[j] 输入 res
			for (int i = 0; i < j + 1; ++i) 
			{ 
				res.push_back(arr[i]); 
			}
			return res;
		}
		// 若 j 小于 k - 1，将区间变成 [j + 1, right]；反之，区间变成 [left, j - 1]
		return j < index ? quickSelection(arr, j + 1, right, index) : quickSelection(arr, left, j - 1, index);
	}
	int partition(vector<int>& arr, int left, int right)
	{
		int value = arr[left];
		int i = left, j = right + 1;

		while (true)
		{
			while (++i <= right && arr[i] < value);						// 找到从左往右第一个大于等于 value 的下标
			while (--j >= left && arr[j] > value);							// 找到从右往左第一个小于等于 value 的下标
			if (i >= j) 
			{ 
				break; 
			} // 如果找不到，说明已经排好序了，break
			swap(arr[i], arr[j]); // 如果找到了，交换二者
		}
		swap(arr[left], arr[j]); // arr[j]是小于 value 的，这一步使得所有小于下标为 j 的数都在 j 左边

		return j;
	}
	void swap(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}
private:
	int mArr[10];
	vector<int> res;
};