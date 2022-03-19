#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define FIFO1 "/tmp/fifo.1" //pr cw
#define FILE_MODEL (S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH)

void Client(int writer);
void Server(int reader);
int main()
{
    pid_t pid = getpid();
	int readFd,writeFd;

	if(mkfifo(FIFO1,FILE_MODEL)==-1)
	{
		perror("mkpipe1 error\n");
		exit(-1);
	}
    //pid=fork();
	//child
	if(pid==0)
	{
		writeFd=open(FIFO1,O_WRONLY);
		if(writeFd==-1)
		{
			printf("%s\n", "open file error,in wtite");
		}
		else
		{
			Client(writeFd);
		    exit(0);
		}

	}
	else
	{		
		readFd=open(FIFO1,O_RDONLY);
		if(readFd==-1)
		{
			printf("%s\n", "open file error,in read");
		}
		else
		{
           unlink(FIFO1);
		   Server(readFd);
		}

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
void Server(int reader)
{
	printf("server start...\n");
	char buffer[1024];
	int ret;
	while((ret=read(reader,buffer,1024))>0) //read from fifo
	{
		printf("client said:%.*s",ret,buffer);//print to console
	}
	close(reader);
}
