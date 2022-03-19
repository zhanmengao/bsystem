#include<stdio.h>

int main()
{
	int val = 0x01234567;
    char data = val;

    if (data == 0x67)
    {
        printf("small \n");
    }
    else
    {
        printf("big \n");
    }
    return 0;
}