#pragma once
#include<algorithm>
#include<vector>
#include<math.h>
using namespace std;

class Solution
{
public:
	Solution(vector<int>& nums) :arr(nums)
	{

	}

	/** Resets the array to its original configuration and return it. */
	vector<int> reset()
	{
		return arr;
	}

	/** Returns a random shuffling of the array. */
	vector<int> shuffle()
	{
		std::vector<int> sortArr = arr;
		std::sort(sortArr.begin(), sortArr.end(), [](int a, int b)
		{
			srand(a + b);
			bool isLess = rand() % 2;
			return isLess;
		});
		return sortArr;
	}

	/** Returns a random shuffling of the array. */
	vector<int> shuffle2()
	{
		std::vector<int> sortArr = arr;
		for (size_t i = 0; i < sortArr.size(); i++)
		{
			srand(sortArr[i]);
			int idx = rand() % sortArr.size();
			std::swap(sortArr[i], sortArr[idx]);
		}
		return sortArr;
	}
private:
	std::vector<int> arr;
};

/**
* Your Solution object will be instantiated and called as such:
* Solution* obj = new Solution(nums);
* vector<int> param_1 = obj->reset();
* vector<int> param_2 = obj->shuffle();
*/
