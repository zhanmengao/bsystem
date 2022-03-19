#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,const char* argv[])
{
	if(argc <3)
	{
		printf("argclist error\n");
		exit(-1);
	}
	int c = 0;
	FILE* fpr = fopen(argv[1],"r");
	if(fpr == NULL)
	{
		perror(argv[1]);
		exit(-1);
	}
	FILE* fpw = fopen(argv[2],"w");
	if(fpw == NULL)
	{
		perror(argv[2]);
		exit(-1);
	}
	while((c = fgetc(fpr)) != EOF)
	{
		if(fputc(c,fpw) == EOF)
		{
			perror("putc");
			continue;
		}
	}
	fclose(fpr);
	fclose(fpw);	
	return 0;
}
