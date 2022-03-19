#include <stdio.h>

int main()
{
    int nNum = 1, nSum = 0;

    do 
    {
        nSum += nNum;
        nNum ++;
    } while ( nNum <= 100 );

    printf("nSum = %d \r\n", nSum);

    return 0;
}