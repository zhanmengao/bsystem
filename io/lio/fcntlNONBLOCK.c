#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>

int main()
{
	int flag = fcntl(STDIN_FILENO,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,flag);
	char buffer[1024] = {0};
	for(int i = 0;i<5;i++)
	{

		int rn = read(STDIN_FILENO,buffer,sizeof(buffer));
		if(rn <0)
		{
			if(errno == EAGAIN)\
			{
				printf("try again %d \n",i);
				sleep(2);
				continue;
			}
			perror("read STDIO_FILENO");
			exit(-1);
		}
		else
		{
			write(STDOUT_FILENO,buffer,rn);
			i = 0;
		}
	}
	return 0;
	return 0;
}

