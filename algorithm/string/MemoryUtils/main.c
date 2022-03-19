#include"mstring.h"
#include<string.h>
#include<stdio.h>

int main()
{
	char buffer[1024] = { 0 };
	mstrcpy(buffer, "OKOK");
	printf("%s \n", buffer);

	{
		char src[] = { "OKOK" };
		strcpy(src, src + 1);
		printf("%s \n", src);
	}
	{
		char src[] = { "OKOK" };
		mstrcpy(src, src + 1);
		printf("%s \n", src);
	}
	{
		char src[] = { "wby" };
		mmemcpy(buffer, src, 5);
		printf("%s \n", buffer);
	}
	{
		char src[] = { "OKOK" };
		memmove(src, src + 1, 5);
		printf("%s \n", src);
	}
	return 0;
}