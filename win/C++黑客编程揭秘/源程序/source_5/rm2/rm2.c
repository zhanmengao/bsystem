#include <stdio.h>

int main()
{
    int a = 0, b = 1, c = 2;

    if ( a > b )
    {
        printf("%d \r\n", a);
    } 
    else if ( b <= c )
    {
        printf("%d \r\n", b);
    }
    else
    {
        printf("%d \r\n", c);
    }

    return 0;
}