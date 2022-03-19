#include"Coin.h"


int main()
{
	int arr[] = { 1,2,5 };
	//coinChange(arr, 3, 11);
	dpSolution dp;
	dp.coinChange(std::vector<int>{1, 2, 5}, 11);
	return 0;
}