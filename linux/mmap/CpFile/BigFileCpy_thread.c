#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include<unistd.h>
#include<fcntl.h>

#define MB 1048576
#define READWRITE PROT_WRITE|PROT_READ
#define READ PROT_READ
#define WRITE PROT_WRITE

char *addr_r ,*addr_w;
int childCount = 0;
int length;
pthread_mutex_t mx;
void* RoutinueMain(void* para)
{
	pthread_mutex_lock(&mx);
	int i = (int)para;
	int ret;
    char* r = addr_r + i * MB;
    char* w = addr_w + i * MB;
	if(i < childCount - 1)
	{
		ret = MB;
	}
	else
	{
        ret = length -  (childCount -1) * MB;
	}
    memcpy(w, r, ret);
	printf("child %d write end\n", i);
	pthread_mutex_unlock(&mx);
	return 0;
}


int main(int argc,char  *argv[])
{
	int fd_r,fd_w;
	pthread_t tid[100];
	if(argc <3)
	{
		return 0;
	}
	char* src = argv[1];
	char* dest = argv[2];
		
	pthread_mutex_init(&mx ,NULL);
	fd_r = open(src,O_RDWR);
	fd_w = open(dest,O_RDWR|O_CREAT,0644);
	if(fd_r && fd_w)
	{
		length=lseek(fd_r,0L,SEEK_END);
		ftruncate(fd_w,length);
        childCount = length%MB == 0? length/MB:length/MB+1;

		addr_w=mmap(NULL,length,READWRITE,MAP_SHARED,fd_w,0);
		addr_r=mmap(NULL,length,READWRITE,MAP_SHARED,fd_r,0);
		close(fd_r);
		close(fd_w);
		if(addr_w == MAP_FAILED || addr_w == NULL || addr_r == MAP_FAILED|| addr_r == NULL)
		{	
			printf("error,r=%x,w=%x\n",addr_r,addr_w);

			if(addr_r==NULL)
			{
				perror("mmap error,r=NULL\n");	
			}
			if(addr_w==NULL)
			{
				perror("mmap error,w=NULL\n");	
			}
			munmap(addr_r,length);
			munmap(addr_w,length);
			exit(0);
		}
		printf("length = %u,w = %x,r = %x\n",length,addr_w,addr_r);

		for(int i = 0;i < childCount;i++)
		{
			//创建线程
			pthread_create(tid + i, NULL, RoutinueMain,(void*)i);
		}
		//回收线程
		for(int i = 0;i< childCount;i++)
		{
			if(pthread_join(tid[i],NULL) == 0)
			{
			    //每join成功一次，进度条++
			    printf("%s","==");
			}
		}
		printf("\n%s\n","文件拷贝成功!");
		munmap(addr_r,length);
		munmap(addr_w,length);
	}
	else
	{
		printf("%s\n","pf == null");
	}
	pthread_mutex_destroy(&mx);
	return 0;
}
