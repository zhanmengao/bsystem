#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	pid_t pid;
	pid=fork();
	//father
	if(pid>0)
	{
		sleep(1);
		//set child process group
		setpgid(pid,pid);
		sleep(5);
		printf("parent PID == %d\n", getpid());
        printf("parent's parent process PID == %d\n", getppid());
        printf("parent Group ID == %d\n", getpgid(0));
        setpgid(getpid(),getppid());
        printf("after setpgid,parent Group ID == %d\n", getpgid(0));
	}
	else
	{
		printf("%s %u\n","I am child,I process group=",getpgid(getpid()));
		sleep(3);
		printf("%s %u\n","I am child,I process group=",getpgrp());		
	}
	return 0;
}