#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>

int main()
{
	int fd[2];
	pid_t pid;
	int ret=pipe(fd);
	if(ret==-1)
	{
		perror("pipe error\n");
		exit(22);
	}
	pid=fork();
	//child read
	if(pid==0)
	{
		close(fd[1]);
		char buffer[1024];
		ret=read(fd[0],buffer,sizeof(buffer));
		if(ret==0)
		{
			printf("--------------------------------------\n");
		}
		write(STDOUT_FILENO,buffer,ret);
	}
	//parent write
	else
	{
		sleep(1);
		char str[12]="hello pipe\n";
		close(fd[0]);
		write(fd[1],str,strlen(str));
	}
}
