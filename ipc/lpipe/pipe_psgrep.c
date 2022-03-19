#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>

int main()
{
	int fd[2];
	int fd2[2];
	pid_t pid;
	pid_t cpid[2];
	int ret=pipe(fd);
	int ret2=pipe(fd2);
	if(ret==-1 || ret2==-1)
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
		close(fd2[1]);
		close(fd2[0]);
	    dup2(fd[1],STDOUT_FILENO);//输出重定向
	    if(execlp("ps","ps","-anx",NULL)==-1)
	    {
	    	printf("ps command error\n");
	    }

	}


	pid=fork();
	//child2 read write
	if(pid==0)
	{
		printf("create program 2 read,pid=%d\n",getpid());
		close(fd[1]);//read from fd
		close(fd2[0])  ;//write to fd2

		char buffer[1024];
		dup2(fd[0],0);//input重定向
		dup2(fd2[1],1);//output 
		if(execlp("grep","grep","uzi",NULL)==-1)
	    {
	    	printf("wc command error\n");
	    }
	}


	pid=fork();
	//child3 read
	if(pid==0)
	{
		printf("create program 3 read,pid=%d\n",getpid());
		close(fd[1]);
		close(fd[0]);
		close(fd2[1]);

		char buffer[1024];
		dup2(fd2[0],0);//input重定向
		if(execlp("wc","wc","-l",NULL)==-1)
	    {
	    	printf("wc command error\n");
	    }
	}



	int i;
	close(fd[1]);
	close(fd[0]);
	close(fd2[1]);
	close(fd2[0]);

	//parent gc
	for(i=0;i<3;i++)
	{
		pid=waitpid(-1,NULL,WNOHANG);
		if(pid)
		{
			printf("gc child %d\n", pid);
		}
	}
}
