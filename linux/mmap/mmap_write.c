#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>

#define MMAPFILE "/tmp/data.1"

struct User
{
	int id;
	char name[32];
};
int main()
{
	int fd;
	struct User* add;
	struct User user;
    fd=open(MMAPFILE,O_RDWR);
	if(fd<0)
	{
		perror("open error");
		exit(-1);
	}
	ftruncate(fd,sizeof(user));
	user.id=10;
	strcpy(((char*)&user)+4,"libaiji");
	printf("sizeof=%lu\n",sizeof(user));
	add=mmap(NULL,sizeof(user),PROT_WRITE,MAP_SHARED,fd,0);// creatt 36 mapp
	if(add==MAP_FAILED)
	{
		perror("mmap error");
		exit(-3);
	}
	while(1)
	{
		user.id = getpid();
		memcpy(add,&user,sizeof(user));
		sleep(5);
	}
	return 0;
}
