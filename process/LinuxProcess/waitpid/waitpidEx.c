#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void printStatus(int status)
{
	if(WIFEXITED(status))
	{
		printf("exit code=%d\n",WEXITSTATUS(status));
	}
	else if(WIFSIGNALED(status))
	{
		printf("err code=%d\n",WTERMSIG(status));
	}
}

int main()
{
	pid_t pid;
	pid=fork();//1
	//child
	if(pid==0)
	{
		printf("do child 1\n");
		execlp("ps","ps","-anx",NULL);
		exit(1);
	}
	//father
	else 
	{
		int status;
		waitpid(pid,&status,0);
		printf("%d is delete:",pid);
		printStatus(status);

	}

	pid=fork();//2
	if(pid==0)
	{
		printf("do child 2\n");
		if(execl("/home/uzi/桌面/gitSPace/系统编程/environ.out",NULL)==-1)
		{
			printf("child 2 find error\n");
			exit(2);
		}
	}
	else
	{
		int status;
		waitpid(pid,&status,0);
		printf("%d is delete:",pid);
		printStatus(status);
	}

	pid=fork();//3
	if(pid==0)
	{
		printf("do child 3\n");
		if(execl("/home/uzi/err.out","/home/uzi/err.out",NULL)==-1)
		{
			printf("child 3 find error\n");
			exit(3);
		}
		
	}
	else
	{
		int status;
		waitpid(pid,&status,0);
		printf("%d is delete:",pid);
		printStatus(status);
	}
}
