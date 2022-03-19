#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
void PrintfBinary(int num)
{
	int flag = 1;
	printf("%d(int):		", num);
	int bit = 0;
	while (flag && ++bit)
	{
		if (num & flag)
		{
			printf("%u", 1);
		}
		else
		{
			printf("%u", 0);
		}
		if(bit%8 == 0)
		{
			printf(" ");
		}
		flag = flag << 1;
	}
	printf("\n");
}
void PrintfBinaryRevert(int num,int bitidx)
{
	if(bitidx == 0)
	{
		printf("%d(int):		", num);	
	}
	int flag = 1;
	flag = flag << bitidx;
	if(bitidx < 31)
	{
		PrintfBinaryRevert(num,bitidx + 1);
	}
	if (num & flag)
	{
		printf("%u", 1);
	}
	else
	{
		printf("%u", 0);
	}
	if(bitidx%8 == 0)
	{
		printf(" ");
	}
}
int main()
{
	int num = 9;
	PrintfBinary(num);
	PrintfBinaryRevert(num,0);
	printf("\n");
    return 0;
}