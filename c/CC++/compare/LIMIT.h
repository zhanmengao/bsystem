#pragma once
#include<limits.h>
#include<stdio.h>
#include<float.h>

void LimitMain()
{
	printf("char min = %d max = %d \n", CHAR_MIN, CHAR_MAX);
	printf("byte min = %u max = %u \n", 0, UCHAR_MAX);
	printf("short min = %d max = %d \n", SHRT_MIN, SHRT_MAX);
	printf("ushort min = %u max = %u \n", 0, USHRT_MAX);

	printf("int min = %d max = %d \n", INT_MIN, INT_MAX);
	printf("uint min = %u max = %u \n", 0, UINT_MAX);
	printf("long min = %ld max = %ld \n", LONG_MIN,LONG_MAX);
	printf("ulong min = %lu max = %lu \n", 0,ULONG_MAX);

	printf("long long min = %lld max = %lld \n", LLONG_MIN,LLONG_MAX);
	printf("u long long min = %u max = %Illu \n", 0,ULLONG_MAX);

	//printf("float min = %d max = %d \n", NDR_VAX_FLOAT, CHAR_MAX);
	//printf("byte min = %u max = %u \n", 0, UCHAR_MAX);
}