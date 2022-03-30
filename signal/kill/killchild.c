#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define CHILDCOUNT 5

int main(int argc, char const *argv[])
{
	pid_t pid;
	for(int i=0;i<CHILDCOUNT;i++)
	{
		pid=fork();
		if(pid==0)
		{
			break;
		}
		else
		{
			if(i!=2)
			{
				continue;
			}
			int ret=kill(pid,SIGKILL);
			if(ret==-1)
			{
				perror("kill error");
				exit(ret);
			}
			else
			{
				printf("pid=%d,%s\n",pid,"index==3 is died");
			}
			return 0;
		}
	}

}