#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>


#define MMAPFILE "/tmp/mmap"

#define FILESIZE 4
int main()
{
	int fd,ret;
	char* add;
	fd=open(MMAPFILE,O_CREAT|O_RDWR);
	if(fd>0)
	{
		ret=ftruncate(fd,FILESIZE);
		if(ret==0)
		{
			add=mmap(NULL,FILESIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);// creatt 4b mapp
			if(add!=MAP_FAILED)
			{
				strcpy(add,"aaa");
				munmap(add,FILESIZE); //delete mapp
			}
			else
			{
				printf("%s\n", "mmap error\n");
			}
		}
		else
		{
			printf("%s\n", "fturncate error\n");
		}
		close(fd);
	}
	else
	{
		printf("%s\n", "open error\n");
	}
	return 0;
}