#pragma once
#include<vector>
#include<stdint.h>
#include<math.h>
#include<algorithm>
/*
动态规划不是某一种具体的算法，而是一种算法思想：若要解一个给定问题，
我们需要解其不同部分（即子问题），再根据子问题的解以得出原问题的解。
*/
using namespace std;
class dpSolution
{
	vector<int>count;
	int dp(vector<int>& coins, int rem)
	{
		if (rem < 0) 
			return -1;
		if (rem == 0)
			return 0;
		if (count[rem - 1] != 0) 
			return count[rem - 1];
		int Min = INT_MAX;
		for (int coin : coins)
		{
			int res = dp(coins, rem - coin);
			if (res >= 0 && res < Min)
			{
				Min = res + 1;
			}
		}
		count[rem - 1] = Min == INT_MAX ? -1 : Min;
		return count[rem - 1];
	}
public:
	int coinChange(vector<int>& coins, int amount)
	{
		if (amount < 1) return 0;
		count.resize(amount);
		return dp(coins, amount);
	}
};

/*
我们采用自下而上的方式进行思考。仍定义 F(i)F(i)F(i) 为组成金额 iii 所需最少的硬币数量。
假设在计算 F(i)F(i)F(i) 之前，我们已经计算出 F(0)?F(i?1)F(0)-F(i-1)F(0)?F(i?1) 的答案。
*/
int coinChange(vector<int>& coins, int amount)
{
	int Max = amount + 1;
	vector<int> dp(amount + 1, Max);
	dp[0] = 0;
	for (int i = 1; i <= amount; ++i)
	{
		for (int j = 0; j < (int)coins.size(); ++j)
		{
			if (coins[j] <= i)
			{
				dp[i] = std::min(dp[i], dp[i - coins[j]] + 1);
			}
		}
	}
	return dp[amount] > amount ? -1 : dp[amount];
}