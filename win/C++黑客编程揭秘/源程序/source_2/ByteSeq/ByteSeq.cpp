// ByteSeq.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main(int argc, char* argv[])
{
    DWORD dwSmallNum = 0x01020304;

    if ( *(BYTE *)&dwSmallNum == 0x04 )
    {
        printf("Small Sequence. \r\n");
    }
    else
    {
        printf("Big Sequence. \r\n");
    }

//     if ( dwSmallNum == htonl(dwSmallNum) )
//     {
//         printf("Big Sequence. \r\n");
//     } 
//     else
//     {
//         printf("Small Sequence. \r\n");
//     }

    return 0;
}
