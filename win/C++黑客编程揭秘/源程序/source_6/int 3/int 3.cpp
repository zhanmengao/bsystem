// int 3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
    __asm int 3

	return 0;
}
// #define STATUS_BREAKPOINT ((NTSTATUS)0x80000003L) 
