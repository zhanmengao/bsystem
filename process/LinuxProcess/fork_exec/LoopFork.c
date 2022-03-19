#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main()
{
	pid_t pid;
    int i;
    for(i=0;i<5;i++)
    {
        pid=fork();
  	    if(pid==-1)
	    {
		   perror("fork error\n");
		   exit(1);
	    }
  	    else if(pid==0)
	    {
		   sleep(i+1);
		   printf("我是子进程,pid=%u,ppid=%u.\n",getpid(),getppid());
           break;
  	    }
	    else
  	    {
		   printf("我是父进程,pid=%u,我创建了子进程=%u.\n",getpid(),pid);
  	    }
	}
	return 0;
}
