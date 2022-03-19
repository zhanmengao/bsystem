#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>

int main()
{
	int fd = open("/dev/tty",O_RDWR|O_NONBLOCK);
	if(fd <0)
	{
		perror("open /dev/tty");
		exit(-1);
	}
	char buffer[1024] = {0};
	for(int i = 0;i<5;i++)
	{

		int rn = read(fd,buffer,sizeof(buffer));
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
			write(fd,buffer,rn);
			i = 0;
		}
	}
	return 0;
}

