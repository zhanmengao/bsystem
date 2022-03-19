#include <stdio.h>

int main()
{
    int nNum = 0;
    scanf("%d", &nNum);

    switch ( nNum )
    {
    case 1:
        {
            printf("1 \r\n");
            break;
        }
    case 2:
        {
            printf("2 \r\n");
            break;
        }
    case 3:
        {
            printf("3 \r\n");
            break;
        }
    case 4:
        {
            printf("4 \r\n");
            break;
        }
    default:
        {
            printf("default \r\n");
            break;
        }
    }

    return 0;
}