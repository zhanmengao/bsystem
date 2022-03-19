#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
int main(int argc,char* argv[])
{
	//只读方式打开一个文件
	int fd = open("flock.c",O_RDONLY);
	printf("fd = %d \n",fd);

	close(fd);

	//打开一个不存在的文件
	fd = open("fopen1.txt",O_RDONLY);
	printf("fd = %d ,errno = %d:%s\n",fd,errno,strerror(errno));
	

	//创建一个文件
	fd = open("fopen.txt",O_RDONLY|O_CREAT,0644);
	char buf[]="qaqqqqqqqqqqqqqqqqqqqqq";
	write(fd,buf,strlen(buf) + 1);
	close(fd);

	//打开文件，如果文件存在，则截断文件并重置成0
	//如果文件不存在，则创建文件
	fd = open("fopen.txt",O_RDONLY|O_CREAT|O_TRUNC,0644);
	return 0;
}
