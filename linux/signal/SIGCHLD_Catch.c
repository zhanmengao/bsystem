#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define CHILD_COUNT 10

void DoSigchild(int signo)
{
	int status;
	pid_t pid;
	while ((pid = waitpid(0, &status, WNOHANG)) > 0)                          //这里用while，处理在执行过程中到来的全部信号
	{
		if (WIFEXITED(status))
		{
			printf("child %d exit %d\n", pid, WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
		}
	}
}

void SigMask2PCB(int signal,int isMask,sigset_t *old)                   //设置pcb屏蔽信号集 工具函数
{
	if(isMask)
	{
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set,signal);
		sigprocmask(SIG_BLOCK,&set,old);
	}
	else
	{
		sigset_t set;
		sigfillset(&set);
		sigdelset(&set,signal);
		sigprocmask(SIG_UNBLOCK,&set,old);
	}
}



int main(int argc, char const *argv[])
{
	pid_t pid;
	for(int i=0;i<CHILD_COUNT;i++)
	{
		pid=fork();
		if(pid==0)
		{
			break;
		}
	}

	if(pid==0)
	{
		int n=1;
		while(n--)
		{
			printf("I am child ,my pid= %u\n", getpid());
		    sleep(1);			
		}
	}
	else
	{
		SigMask2PCB(SIGCHLD,1,NULL);
		signal(SIGCHLD,DoSigchild);
		SigMask2PCB(SIGCHLD,0,NULL);
		while(1)
		{
			printf("I am parent,my pid= %u \n",getpid());
			sleep(1);
		}
	}

	return 0;
}