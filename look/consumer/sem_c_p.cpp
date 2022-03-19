#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <iostream>
#include <queue>
#include <forward_list>
using namespace std;

#define CONSUMER_COUNT 5
#define MAX 500

typedef struct 
{
	int sender;
	int msgId;
}Message;

sem_t boxSem;
sem_t itemSem;
queue<Message> cache;
//生产者：产生数据
void* ProducerMain(void* para)
{
	while(1)
	{
		sem_wait(&boxSem);
		Message msg;
		msg.sender = rand() % 999999;
		msg.msgId = rand() % 1000;
		cache.push(msg);
		sem_post(&itemSem);
		sleep(rand() % 2);		
	}
	return NULL;
} 

//消费者：处理数据
void* ConsumerMain(void* para)
{
	while(1)
	{
		sem_wait(&itemSem);
		if(cache.empty())
		{
			perror("data exception!");
			exit(-1);
		}
		Message msg = cache.front();
		cache.pop();
		sem_post(&boxSem);
		printf("receive a package,sender = %u msgid = %u\n",msg.sender,msg.msgId);
		sleep(rand() % 2);
	}
	return NULL;
} 

int main(int argc, char const *argv[])
{
	pthread_t pid;
	pthread_t cid[CONSUMER_COUNT];
	sem_init(&boxSem,0,MAX);                          //一开始有max个盒子
	sem_init(&itemSem,0,0);                           //一开始有0个产品 
	pthread_create(&pid,NULL,ProducerMain,NULL);
	for(int i = 0;i<CONSUMER_COUNT;i++)
	{
	    pthread_create(cid+i,NULL,ConsumerMain,NULL);		
	}

	//join
	pthread_join(pid,NULL);
	for(int i = 0;i<CONSUMER_COUNT;i++)
	{
	    pthread_join(cid[i],NULL);		
	}	
	return 0;
}