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
		return quickSelection(arr, 0, arr.size() - 1, k - 1);								// ע��� k ������Ӧ���±��� k - 1
	}
	vector<int> quickSelection(vector<int>& arr, int left, int right, int index)
	{
		// partition������һ������������С���±�Ϊ j �������� j ��ߣ������±�Ϊ j �������� j �ұ�
		int j = partition(arr, left, right);

		if (j == index)
		{ 
			// �� j �պõ��� k - 1���� arr[0] �� arr[j] ���� res
			for (int i = 0; i < j + 1; ++i) 
			{ 
				res.push_back(arr[i]); 
			}
			return res;
		}
		// �� j С�� k - 1���������� [j + 1, right]����֮�������� [left, j - 1]
		return j < index ? quickSelection(arr, j + 1, right, index) : quickSelection(arr, left, j - 1, index);
	}
	int partition(vector<int>& arr, int left, int right)
	{
		int value = arr[left];
		int i = left, j = right + 1;

		while (true)
		{
			while (++i <= right && arr[i] < value);						// �ҵ��������ҵ�һ�����ڵ��� value ���±�
			while (--j >= left && arr[j] > value);							// �ҵ����������һ��С�ڵ��� value ���±�
			if (i >= j) 
			{ 
				break; 
			} // ����Ҳ�����˵���Ѿ��ź����ˣ�break
			swap(arr[i], arr[j]); // ����ҵ��ˣ���������
		}
		swap(arr[left], arr[j]); // arr[j]��С�� value �ģ���һ��ʹ������С���±�Ϊ j �������� j ���

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