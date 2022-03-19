#pragma once
#include<iostream>
#include<vector>
#include<unordered_set>
#include<map>
#include<unordered_map>
using namespace std;
typedef struct
{
	std::vector<int> data;
	void Add(int num)
	{
		data.push_back(num);
	}
	bool Find(int num)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			for (size_t j = 0; j < data.size(); j++)
			{
				if (i == j)
				{
					continue;
				}
				if (data[i] + data[j] == num)
				{
					return true;
				}
			}
		}
		return false;
	}
}TwoSum;

class TwoSumPlus
{
private:
	//容器：可重复，0(1)
	std::unordered_set<int> data;
public:
	void Add(int num)
	{
		data.insert(num);
	}
	bool Find(int num)
	{
		for (int val : data)
		{
			//看需要几个这样的数，避免6=3+3（同一个3）
			int need = 1;
			int diff = num - val;
			if (diff == val)
			{
				need++;
			}
			if (data.count(diff) >= need)
			{
				return true;
			}
		}
		return false;
	}
};

class MapSolution
{
public:
	std::vector<int> twoSum(vector<int>& nums, int target)
	{
		vector<int> ret;
		for (int i = 0; i<nums.size(); i++)
		{
			data[nums[i]].push_back(i);
		}
		for (int i = 0; i<nums.size(); i++)
		{
			int val = target - nums[i];
			auto fi = data.find(val);
			if (fi != data.end())
			{
				for (auto it = fi->second.begin(); it != fi->second.end(); it++)
				{
					if (*it != i)
					{
						ret.push_back(i);
						ret.push_back(*it);
						return ret;
					}
				}
			}
		}
		return ret;
	}
private:
	std::unordered_map<int, std::list<int>> data;
};

void TwoSummain()
{
	TwoSumPlus ts;
	ts.Add(3);
	ts.Add(4);
	ts.Add(6);
	std::cout << ts.Find(7) << std::endl;
	std::cout << ts.Find(6) << std::endl;
}
