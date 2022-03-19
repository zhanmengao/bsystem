#include"libSystemInfo.h"
#include<stdio.h>
void main()
{
	char buffer[1024] = { 0 };
	int len;
	libGetOSFullName(buffer, len);
	printf("OS Full Name %s \n", buffer);
}