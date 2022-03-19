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

sem_t boxSem;
sem_t itemSem;
queue<string> cache;
//生产者：产生数据
void* ProducerMain(void* para)
{
	string str;
	struct timespec abs_timeout;
	while(1)
	{
		abs_timeout.tv_sec = time(NULL) + 1;
		abs_timeout.tv_nsec = 0;
		if(sem_timedwait(&boxSem,&abs_timeout) != 0)
		{
			printf("%s\n","容器大小不足!");
			continue;
		}
		//接受输入，写入到公共数据区
		cin>>str;
		cout<<endl;
		cache.push(str);
		sem_post(&itemSem);	
	}
	return NULL;
} 

//消费者：处理数据
void* ConsumerMain(void* para)
{
	struct timespec abs_timeout;
	while(1)
	{
		abs_timeout.tv_sec = time(NULL) + 5;
		abs_timeout.tv_nsec = 0;
		if(sem_timedwait(&itemSem,&abs_timeout)!= 0)              //检测公共数据区有没有数据，没有则打印hello world
		{
			printf("system said: %s\n","hello world!");
			continue;
		}
		string str = cache.front();
		cache.pop();
		sem_post(&boxSem);
		cout<<"client said: "<<str<<endl;
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