#pragma once


static int RevertNum(int num)
{
	int v1 = num % 10;
	int v2 = (num/10) % 10;
	int v3 = num / 100;
	return v1*100 + v2*10 + v3;
}