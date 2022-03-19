#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
typedef unsigned char *byte_pointer;
void PrintBytes(byte_pointer start, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		printf("%p : %.2x \n",&start[i], start[i]);
	}
	printf("\n");
}

int main()
{
    int num = 9;
	int num2 = 0x87654321;
	int big = htonl(num2);
    PrintBytes((byte_pointer)&num,sizeof(num));
	PrintBytes((byte_pointer)&num2,sizeof(num2));
	PrintBytes((byte_pointer)&big,sizeof(big));
    return 0;
}