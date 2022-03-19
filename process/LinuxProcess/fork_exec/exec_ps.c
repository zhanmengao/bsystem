#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int fd;
	fd=open("ps.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
	if(fd<0)
	{
		perror("open file error");
		exit(1);
	}
	dup2(fd,STDOUT_FILENO);//输出重定向
	execlp("ps","-anx","-anx",NULL);
    perror("exec error");
    exit(1);
	return 0;
}

