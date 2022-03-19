#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include<unistd.h>

#define MB 1048576

int main(int argc, char *argv[])
{
	int fd_r,fd_w;
	int pid;
    if(argc <3)
    {
        return -1;
    }
    char* src = argv[1];
    char* dest = argv[2];
    fd_r=open(src,O_RDONLY);
    fd_w=open(dest,O_RDWR | O_CREAT);
	if(fd_r && fd_w)
	{
		int childCount,length,i;
		char* addr;;
		length=lseek(fd_r,0L,SEEK_END);
		printf("file size=%d\n",length);
		lseek(fd_r,0L,SEEK_SET);
		childCount=length/MB+1;
		ftruncate(fd_w,length);

		addr=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd_w,0);
		close(fd_w);
		if(addr==MAP_FAILED)
		{
			perror("mmap error\n");
			exit(-1);
		}

		for(i=0;i<childCount;i++)
		{
			sleep(1);
			pid=fork();
			if(pid==0)
			{
				char *tempadd;
				char buffer[1024];
                int j,ret;
				tempadd=addr+i*MB;

				lseek(fd_r,(i*MB),SEEK_SET);
				for(j=0;j<1024;j++)
				{
					ret=read(fd_r,buffer,1024);
					if(ret>0)
					{
					    memcpy(tempadd,buffer,ret);
					    tempadd+=ret;
					}
					else
					{
						exit(1);
					}
				}
				printf("child %d read end\n",i);
				exit(1);
			}
		}
		sleep(3);
		munmap(addr,length);
	    close(fd_r);
	}
	else
	{
		printf("%s\n","pf==null");
	}

	return 0;
}
