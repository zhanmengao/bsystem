#include<fcntl.h>
#include<stdio.h>
#include<error.h>
#include<sys/stat.h>
#include<unistd.h>

int main()
{
	struct flock lock;
	int fd = open("myfile.txt",O_RDWR|O_CREAT,0777);
	if(fd > 0)
	{
		lock.l_type = F_WRLCK;		//独占锁
		lock.l_whence=SEEK_SET;		//从文件开始位置开始锁
		lock.l_start = 0;		//偏移
		lock.l_len = 0;			//锁到文件末尾
		lock.l_pid = getpid();
		int res = fcntl(fd,F_SETLK,&lock);		//设置文件锁
		printf("return value of fcntl = %d \n",res);
		while(1)
		{
			sleep(10000);
		}
	}
	return 0;
}

