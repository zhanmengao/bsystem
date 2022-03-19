#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>


int main(int argc,const char *argv[])
{
	if(argc <3)
	{
		printf("command size error\n");
		return -1;
	}
	char buffer[1024];//1 kb
	memset(buffer,0,sizeof(buffer));
	int fd_r,fd_w;
	int ret;
	fd_r=open(argv[1],O_RDONLY);
	printf("open %s fd = %d \n",argv[1],fd_r);
	fd_w=open(argv[2],O_WRONLY | O_CREAT| O_TRUNC);
	printf("open %s fd = %d \n",argv[2],fd_w);
	if(fd_r && fd_w)
	{
		while((ret=read(fd_r,buffer,1024))>0)
		{
			ret = write(fd_w,buffer,ret);
			printf("write %d bytes \n",ret);
			memset(buffer,0,sizeof(buffer));
		}	
	}
	else
	{
		printf("%s\n","write error");
	}
	return 0;
}
