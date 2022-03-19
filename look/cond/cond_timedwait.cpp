#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <forward_list>
using namespace std;

#define CONSUMER_COUNT 5

typedef struct 
{
	int sender;
	int msgId;
}Message;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue<Message> cache;
//生产者：产生数据
void* ProducerMain(void* para)
{
	forward_list<Message> buffer;
	buffer.clear();
	while(1)
	{
		int count = rand() % 10;
		for(int i=0;i<count;i++)
		{
			Message msg;
			msg.sender = rand() % 999999;
			msg.msgId = rand() % 1000;
			buffer.push_front(msg);
		}
		//加锁 往公共区域放入请求
		pthread_mutex_lock(&mutex);
		for(auto it = buffer.begin();it!=buffer.end();it++)
		{
			cache.push(*it);
		}
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);                  //通知消费者线程处理请求        
		buffer.clear();
		sleep(rand() % 2);		
	}
	return NULL;
} 

//消费者：处理数据
void* ConsumerMain(void* para)
{
	struct timespec  abstime;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(cache.empty())                     //如果为空，就wait。如果不为空，则直接往下执行
		{
			abstime.tv_sec = time(NULL) + 1;
			abstime.tv_nsec = 0;
			if(pthread_cond_timedwait(&cond,&mutex,&abstime)!= 0)         //形参：绝对时间
			{
				printf("%s\n","没等到消息 我去做其他事了！");
				sleep(1);
			}
			//否则等到消息了，mutex重新加锁成功，继续往下执行
		}
		Message msg = cache.front();
		cache.pop();
		pthread_mutex_unlock(&mutex);	
		
		printf("receive a package,sender = %u msgid = %u\n",msg.sender,msg.msgId);
		sleep(rand() % 2);
	}
	return NULL;
} 

int main(int argc, char const *argv[])
{
	pthread_t pid;
	pthread_t cid[CONSUMER_COUNT];
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