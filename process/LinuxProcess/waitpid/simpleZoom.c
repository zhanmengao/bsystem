#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	pid_t pid;
	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	pid=fork();
	if(pid==-1)
	{
		perror("fork error\n");
		exit(1);
	}
	else if(pid==0)
	{
		printf("我是子进程,pid=%u,ppid=%u.\n",getpid(),getppid());
	}
	else
	{
                if(wait(NULL)!=-1)
                {
                   printf("僵尸进程%d 被回收\n",pid);
                }
		printf("我是父进程,pid=%u,ppid=%u.\n",getpid(),getppid());
	}
	return 0;
}
