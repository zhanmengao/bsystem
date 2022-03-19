#include <stdio.h>

int main()
{
    int nNum = 0, nSum = 0;

    for ( nNum = 1; nNum <= 100; nNum ++ )
    {
        nSum += nNum;
    }

    printf("nSum = %d \r\n", nSum);

    return 0;
}