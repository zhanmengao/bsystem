#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>


#define MMAPFILE "/tmp/mmap"

#define FILESIZE 4

int val;
int main()
{
	int fd,ret,pid;
	char* add;
	val=100;
	fd=open(MMAPFILE,O_CREAT|O_RDWR);
	if(fd<0)
	{
		perror("open error");
		exit(-1);
	}
	ret=ftruncate(fd,FILESIZE);
	if(ret==-1)
	{
		perror("ftruncate error");
		exit(-2);
	}
	add=mmap(NULL,FILESIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);// creatt 4b mapp
	if(add==MAP_FAILED)
	{
		perror("mmap error");
		exit(-3);
	}
	close(fd);
	pid=fork();
	if(pid==0)
	{
		*add=4;
		val=1000;
		printf("child said:add=%u,val=%u\n",*add,val);
	}
	else
	{
		sleep(2);
		printf("parent said:add=%u,val=%u\n",*add,val);
		munmap(add,FILESIZE); //delete mapp
	}
	return 0;
}