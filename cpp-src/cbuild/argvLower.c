#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc,char ** argv)
{
	printf("agrgc=%d,input para=%s\n", argc,argv[1]);
	if(argc>=3 && strcmp("low" ,argv[1])==0)
	{
		char text=argv[2][0];
		printf("tolower,input=%c,output=",text);
		putchar(tolower(text));
	}
	else if(argc>=3 && strcmp("upp",argv[1])==0)
	{
		char text=argv[2][0];
		printf("toupper,input=%c,output=",text);
		putchar(toupper(text));
	}
	return 9;
}