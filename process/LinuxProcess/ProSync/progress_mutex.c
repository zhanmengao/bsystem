#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

#define FILE_NAME "mt_test"

typedef struct 
{
	int data;
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
}Mutex;

Mutex* pLock = NULL;
void Main1()
{
	while(1)
	{
		pthread_mutex_lock(&pLock->mutex);
		pLock-> data +=20;
		int data = pLock ->data;
		pthread_mutex_unlock(&pLock->mutex);	
		printf("Main1:data = %u\n",data);
		sleep(rand() % 3);
	}
}

void Main2()
{
	while(1)
	{
		pthread_mutex_lock(&pLock->mutex);
		pLock-> data -=10;
		int data = pLock ->data;
		pthread_mutex_unlock(&pLock->mutex);	
		printf("Main2:data = %u\n",data);
		sleep(rand() % 3);
	}
}

int main(int argc, char const *argv[])
{
	//mmap
	int fd = open(FILE_NAME,O_CREAT | O_RDWR, 0777);
	if(fd < 0)
	{
		perror("open file error!");
		exit(-1);
	}
	ftruncate(fd,sizeof(Mutex));
	pLock = mmap(NULL,sizeof(Mutex),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(pLock == MAP_FAILED || pLock == NULL)
	{
		perror("mmap error!");
		exit(-1);
	}
	close(fd);
	memset(pLock,0,sizeof(Mutex));

	//初始化mmap返回的内存地址属性
	pthread_mutexattr_init(&(pLock-> attr));
	pthread_mutexattr_setpshared(&(pLock-> attr),PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&(pLock-> mutex),&(pLock->attr));

	pid_t pid = fork();
	if(pid > 0)
	{
		Main1();
		munmap(pLock,sizeof(Mutex));
		pthread_mutex_destroy(&(pLock-> mutex));
		pthread_mutexattr_destroy(&(pLock-> attr));	
	}
	else if(pid == 0)
	{
		Main2();
	}
	return 0;
}