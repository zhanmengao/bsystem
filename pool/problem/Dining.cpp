#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
using namespace std;

#define COUNT 5
#define TOTAL 10000
typedef struct 
{
	int sender;
	int msgId;
}Message;

queue<Message> cache;
pthread_mutex_t lockArr[COUNT];

void* RoutinueMain(void* para)
{
	int index = (int)para;
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
					printf("routinue %u receive msg,Sender = %u ,msgid = %u\n",index,msg.sender,msg.msgId);
					sleep(rand() % 5);
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
					printf("routinue %u receive msg,Sender = %u ,msgid = %u\n",index,msg.sender,msg.msgId);
					sleep(rand() % 5);
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
	pthread_t tid[COUNT];
	//准备5把锁
	for(int i = 0;i<COUNT;i++)
	{
		pthread_mutex_init(lockArr+i,NULL);
	}
	//准备数据
	for(int i = 0;i<TOTAL;i++)
	{
		Message msg;
		msg.sender = i;
		msg.msgId = i;
		cache.push(msg);
	}
	//5个线程
	for(int i = 0;i<COUNT;i++)
	{
		pthread_create(tid+i,NULL,RoutinueMain,(void*)i);
	}

	//回收
	for(int i = 0;i<COUNT;i++)
	{
		pthread_join(tid[i],NULL);
	}
	for(int i = 0;i<COUNT;i++)
	{
		pthread_mutex_destroy((lockArr[i]));
	}
	return 0;
}