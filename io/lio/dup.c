#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<wait.h>
#include<stdlib.h>

int main()
{
	int fd = open("a.txt",O_RDWR | O_CREAT |O_TRUNC,0644);
	if(fd <0)
	{
		perror("");
		return -1;
	}
	int pid = fork();
	if(pid == 0)
	{
		dup2(fd,STDOUT_FILENO);
		dup2(fd,STDERR_FILENO);
		printf("child said ok \n");
		perror("child error ok \n");
		exit(0);	
	}
	else
	{
		waitpid(pid,NULL,0);
		char buffer[1024]= {0};
		read(fd,buffer,sizeof(buffer));
		printf("%s",buffer);
	}
	return 0;
}
