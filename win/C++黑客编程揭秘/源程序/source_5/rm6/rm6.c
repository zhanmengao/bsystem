#include <stdio.h>

int main()
{
    int nNum = 1, nSum = 0;

    while ( nNum <= 100 )
    {
        nSum += nNum;
        nNum ++;
    }

    printf("nSum = %d \r\n", nSum);

    return 0;
}