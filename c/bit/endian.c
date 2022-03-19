#include<stdio.h>

int GetBit(int flag, int nBitsIdx)
{
	int val = ((flag & (1 << nBitsIdx)));
	return val > 0;
}
int isBigEndian()
{
	int a = 1;
	if (GetBit(a, 0) == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int main()
{
    if (!isBigEndian())
    {
        printf("small \n");
    }
    else
    {
        printf("big \n");
    }
    return 0;
}