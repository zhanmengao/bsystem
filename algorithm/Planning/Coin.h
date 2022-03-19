#pragma once
#include<vector>
#include<stdint.h>
#include<math.h>
#include<algorithm>
/*
��̬�滮����ĳһ�־�����㷨������һ���㷨˼�룺��Ҫ��һ���������⣬
������Ҫ���䲻ͬ���֣��������⣩���ٸ���������Ľ��Եó�ԭ����Ľ⡣
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
���ǲ������¶��ϵķ�ʽ����˼�����Զ��� F(i)F(i)F(i) Ϊ��ɽ�� iii �������ٵ�Ӳ��������
�����ڼ��� F(i)F(i)F(i) ֮ǰ�������Ѿ������ F(0)?F(i?1)F(0)-F(i-1)F(0)?F(i?1) �Ĵ𰸡�
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