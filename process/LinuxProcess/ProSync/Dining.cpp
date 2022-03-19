#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <queue>
using namespace std;

#define COUNT 5
#define TOTAL 10000
#define FILE_NAME "mt_test"
#define SIZE sizeof(pthread_mutex_t)*COUNT
typedef struct 
{
	int sender;
	int msgId;
}Message;
void printStatus(int status)
{
	if(WIFEXITED(status))
	{
		printf("exit code=%d\n",WEXITSTATUS(status));
	}
	else if(WIFSIGNALED(status))
	{
		printf("err code=%d\n",WTERMSIG(status));
	}
}
queue<Message> cache;
pthread_mutex_t* lockArr;

void ProgressMain(int index)
{
	while(!cache.empty())
	{
	    //0 - 3这四个孩子先获取+1的锁
		if(index <=3)
		{
			//右手边的锁
			if(pthread_mutex_trylock(lockArr + index + 1) == 0)
			{
				//左手边的锁
				if(pthread_mutex_trylock(lockArr + index) == 0)
				{
					//加锁成功的逻辑
					Message msg = cache.front();
					cache.pop();
					pthread_mutex_unlock(lockArr + index);
					pthread_mutex_unlock(lockArr + index + 1);
				}
				//对左手边的锁加锁失败，则放弃右手边的锁
				else
				{
					pthread_mutex_unlock(lockArr + index +i);
				}
			}
		}
		//4这个孩子先获取4的锁
		else
		{
			//左手边的锁
			if(pthread_mutex_trylock(lockArr + index) == 0)
			{
				//右手边的锁 0号锁
				if(pthread_mutex_trylock(lockArr) == 0)
				{
					//加锁成功的逻辑
					Message msg = cache.front();
					cache.pop();
					pthread_mutex_unlock(lockArr + index);
					pthread_mutex_unlock(lockArr);
				}
				//对左手边的锁加锁失败，则放弃右手边的锁
				else
				{
					pthread_mutex_unlock(lockArr);
				}
			}
		}		
	}
	return NULL;
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
	ftruncate(fd,SIZE);
	lockArr = mmap(NULL,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(lockArr == MAP_FAILED || lockArr == NULL)
	{
		perror("mmap error!");
		exit(-1);
	}
	close(fd);
	//准备5把锁
	pthread_mutexattr_t attr;	
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	for(int i = 0;i<COUNT;i++)
	{
		pthread_mutex_init(lockArr+i,attr);
	}


	//准备数据
	for(int i = 0;i<TOTAL;i++)
	{
		Message msg;
		msg.sender = i;
		msg.msgId = i;
		cache.push(msg);
	}
	//5个进程
	pid_t pid[COUNT];	
	for(int i = 0;i<COUNT;i++)
	{
		pid[i] = fork();
		if(pid[i] == 0)
		{
			ProgressMain(i);
			return;
		}
	}

	//回收
	for(int i = 0;i<COUNT;i++)
	{
		int status;
		waitpid(pid[i],&status,0);
		printf("%d is delete:",pid);
		printStatus(status);
	}
	for(int i = 0;i<COUNT;i++)
	{
		pthread_mutex_destroy((lockArr[i]));
	}
	return 0;
}