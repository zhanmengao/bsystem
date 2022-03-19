#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	pid_t pid,wpid;
	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	pid=fork();
	if(pid==-1)
	{
		perror("fork error\n");
		exit(1);//退出值
	}
	else if(pid==0)
	{
		printf("我是子进程,pid=%u,ppid=%u.\n",getpid(),getppid());
                exit(22);
	}
	else
	{
                int status;
                if((wpid=wait(&status))!=-1)
                {
                   printf("僵尸进程%d 被回收\n",pid);
                   if(WIFEXITED(status)!=0) //正常退出
                   {
                       printf("子进程正常退出值为: %d\n",(WEXITSTATUS(status)));
                   }
                }
	}
	return 0;
}
