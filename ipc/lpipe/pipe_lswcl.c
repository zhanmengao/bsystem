#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>

int main()
{
	int fd[2];
	pid_t pid;
	int ret=pipe(fd);
	if(ret==-1)
	{
		perror("pipe error\n");
		exit(22);
	}
	pid=fork();
	//child read
	if(pid==0)
	{
		close(fd[1]);
		char buffer[1024];
		dup2(fd[0],0);//input重定向
		execlp("wc","wc","-l",NULL);//command read from fd[0]
	}
	//parent write
	else
	{
		close(fd[0]);
	    dup2(fd[1],STDOUT_FILENO);//输出重定向
	    execlp("ls","ls",NULL);
	}
}
