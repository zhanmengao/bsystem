#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		perror("input error!Please input fileName!");
		exit(-1);
	}
	int fd = open(argv[1],O_RDWR);
	if(fd < 0)
	{
		perror("filename error!");
		exit(-1);
	}
	struct flock mlock;
	mlock.l_type = F_WRLCK;
	mlock.l_whence = SEEK_SET;
	mlock.l_start = 0;
	mlock.l_len = 0;

	fcntl(fd,F_SETLKW,&mlock);
	printf("lock file %s\n",argv[1]);
	sleep(20);

	mlock.l_type = F_UNLCK;
	fcntl(fd,F_SETLKW,&mlock);
	printf("un flock file %s\n",argv[1]);
	close(fd);
	return 0;
}