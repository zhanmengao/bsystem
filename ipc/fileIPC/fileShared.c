#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define SHAREFILE "/tmp/share.1"


int main()
{
	pid_t pid;
	FILE* pF;
	pid=fork();
	//child
	if(pid==0)
	{
		int ret;
		char buffer[1024];
		pF=fopen(SHAREFILE,"w+");
		setbuf(pF,NULL);
		while(1)
		{
			sleep(1);
			if(fgets(buffer,1024,pF)!=NULL)
			{
			    printf("server said:%s\n",buffer);
			}
		}
	}
	else
	{
		int ret;
		char buffer[1024];
		pF=fopen(SHAREFILE,"w+");
		setbuf(pF,NULL);
		while((ret=read(STDIN_FILENO,buffer,1024))>0)
		{
			fprintf(pF,"%s\n",buffer);
			fflush(pF);
		}
	}
	return 0;
}