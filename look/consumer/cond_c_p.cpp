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
class CondLock
{
private:
	pthread_mutex_t mutex;
	pthread_cond_t waitCond;
	pthread_cond_t postCond;
	queue<Message> cache;
	int maxSize;
public:
	CondLock(int max = 100):maxSize(max)
	{
		pthread_cond_init(&waitCond,NULL);
		pthread_cond_init(&postCond,NULL);		
		pthread_mutex_init(&mutex,NULL);
	}
	~CondLock()
	{
		pthread_cond_destroy(&waitCond);
		pthread_cond_destroy(&postCond);		
		pthread_mutex_destroy(&mutex);		
	}
public:
	void Post(const Message& msg)
	{
		pthread_mutex_lock(&mutex);
		if(maxSize!= -1 && cache.size() >= maxSize)
		{
			pthread_cond_wait(&postCond,&mutex);		
		}
		cache.push(msg);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&waitCond);                  //通知消费者线程处理请求  
	}
	Message Wait()
	{
		pthread_mutex_lock(&mutex);
		while(cache.empty())                     //如果为空，就wait
		{
			pthread_cond_wait(&waitCond,&mutex);
		}
		Message msg = cache.front();
		cache.pop();
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&postCond);                  //通知生产者线程生产数据	
		return msg;
	}
};
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
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(cache.empty())                     //如果为空，就wait
		{
			pthread_cond_wait(&cond,&mutex);
		}
		//不为空 并且wait成功 才会执行到这里
		Message msg = cache.front();
		cache.pop();
		pthread_mutex_unlock(&mutex);	
		
		printf("receive a package,sender = %u msgid = %u\n",msg.sender,msg.msgId);
		sleep(rand() % 8);
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
