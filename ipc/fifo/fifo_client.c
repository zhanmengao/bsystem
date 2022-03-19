#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define FIFO1 "/tmp/fifo.1" //pr cw

void Client(int writer);
int main()
{
	int writeFd;
	writeFd=open(FIFO1,O_WRONLY,0);
	printf("writefd=%d\n",writeFd);
	if(writeFd==-1)
	{
		printf("%s\n","client connect error");
	}
	else
	{
		Client(writeFd);
		close(writeFd);
	}
	return 0;
}

void Client(int writer)
{
	printf("client connect...\n");
	char buffer[1024];
	int ret;
	write(writer,"I client,I am caming",strlen("I client,I am caming"));
	while((ret=read(STDIN_FILENO,buffer,1024))>0)//read from console
	{
		write(writer,buffer,ret);
	}
	close(writer);
}