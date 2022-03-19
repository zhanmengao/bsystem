// MemBreak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

#define MEMLEN  0x100

int main(int argc, char* argv[])
{
    PBYTE pByte = NULL;

    pByte = (PBYTE)malloc(MEMLEN);
    if ( pByte == NULL )
    {
        return -1;
    }

    DWORD dwProtect = 0;
    VirtualProtect(pByte, MEMLEN, PAGE_READONLY, &dwProtect);

    BYTE bByte = '\xCC';

    memcpy(pByte, (const char *)&bByte, MEMLEN);

    free(pByte);

	return 0;
}
