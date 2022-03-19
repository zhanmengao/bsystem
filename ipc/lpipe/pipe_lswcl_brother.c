#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>

int main()
{
	int fd[2];
	pid_t pid;
	pid_t cpid[2];
	int ret=pipe(fd);
	if(ret==-1)
	{
		perror("pipe error\n");
		exit(22);
	}
	pid=fork();
	//child1 write
	if(pid==0)
	{
		printf("create program 1 write,pid=%d\n",getpid());
		close(fd[0]);
	    dup2(fd[1],STDOUT_FILENO);//输出重定向
	    if(execlp("ls","ls",NULL)==-1)
	    {
	    	printf("ls command error\n");
	    }

	}
	else
	{
		cpid[0]=pid;
	}

	pid=fork();
	//child2 read
	if(pid==0)
	{
		printf("create program 2 read,pid=%d\n",getpid());
		close(fd[1]);
		char buffer[1024];
		dup2(fd[0],0);//input重定向
		if(execlp("wc","wc","-l",NULL)==-1)
	    {
	    	printf("wc command error\n");
	    }
	}
	else
	{
		cpid[1]=pid;
	}
	int i;
	//parent gc
	for(i=0;i<2;i++)
	{
		pid=waitpid(cpid[1-i],NULL,WNOHANG);
		if(pid)
		{
			printf("gc child %d\n", pid);
		}
	}
}
