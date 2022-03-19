#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include "memwatch.h"

int getMem(char **myp)
{
	char *p = NULL;

	p = (char *)malloc(100);   //mwMalloc(100,__FILE__,__LINE__)

	strcpy(p, "aaaabbbb");
	*myp = p;
	return 0;
}

void main22333()
{
	char *myp = NULL;
	getMem(&myp);
	printf("%s \n", myp);

	if (myp != NULL)
	{
		free(myp);
	}
	
	printf("hello...\n");
	system("pause");
	return ;
}
