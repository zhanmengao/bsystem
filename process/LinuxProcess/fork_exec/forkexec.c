#include"../../../Linux/apue.h"
#include<sys/wait.h>

int main()
{
	pid_t pid;
	char buf[MAXLINE];
	int status;

	while(fgets(buf,MAXLINE,stdin)!=NULL)
	{
		if(buf[strlen(buf) - 1] == '\n')
		{
			buf[strlen(buf) - 1] = NULL;
		}
		if((pid = fork()) < 0)
		{
			perror("fork error");
		}
		else if(pid == 0)
		{
			execlp(buf,buf,(char*)NULL);
			printf("couldn't execute: %s ",buf);
			exit(127);
		}



		if((pid == waitpid(pid,&status,0)) < 0)
		{
			perror("waitpid");
		}
		printf("----------------------------------\n");
	}
	return 0;
}
