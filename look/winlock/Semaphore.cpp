#include "Define.h" 

unsigned WINAPI Read(void * arg);
unsigned WINAPI Accu(void * arg);

static HANDLE semOne;
static HANDLE semTwo;
static int num;

/*
可以利用“信号量值为0时进入non-signaled状态，大于0时进入signaled状态”的特性进行同步。
向lInitialCount参数传递0时，创建non-signaled状态的信号量对象。
而向lMaximumCount传入3时，信号量最大值为3，因此可以实现3个线程同时访问临界区时的同步。

信号量对象大于0时成为signaled对象，为0时成为non-signaled对象。
因此，调用WaitForSingleObject函数时，信号量大于0的情况下才会返回。
返回的同时将信号量的值减1，同时进入non-signaled状态。
*/
int mainSemaphore()
{
	HANDLE hThread1, hThread2;

	//创建信号量对象,设置为0进入non-signaled状态  
	semOne = CreateSemaphore(NULL, 0, 1, NULL);          //为0锁住，最开始有1个

	//创建信号量对象，设置为1进入signaled状态  
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);          //为1锁住，最开始有1个

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);
	CloseHandle(semOne); //销毁  
	CloseHandle(semTwo); //销毁  

	return 0;
}

//生产者
unsigned WINAPI Read(void * arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		fputs("Input num: \n", stdout);

		//临界区的开始 signaled状态  
		WaitForSingleObject(semTwo, INFINITE);
		scanf("%d", &num);

		//临界区的结束 non-signaled状态
		ReleaseSemaphore(semOne, 1, NULL);
	}
	return 0;
}

//消费者
unsigned WINAPI Accu(void * arg)
{
	int sum = 0, i;
	for (i = 0; i < 5; i++)
	{
		//临界区的开始 non-signaled状态  
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		printf("sum = %d \n", sum);
		//临界区的结束 signaled状态  
		ReleaseSemaphore(semTwo, 1, NULL);
	}
	printf("Result: %d \n", sum);
	return 0;
}

/*
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
	while (1)
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
	while (1)
	{
		sem_wait(&itemSem);
		if (cache.empty())
		{
			perror("data exception!");
			exit(-1);
		}
		Message msg = cache.front();
		cache.pop();
		sem_post(&boxSem);
		printf("receive a package,sender = %u msgid = %u\n", msg.sender, msg.msgId);
		sleep(rand() % 2);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t pid;
	pthread_t cid[CONSUMER_COUNT];
	sem_init(&boxSem, 0, MAX);                          //一开始有max个盒子
	sem_init(&itemSem, 0, 0);                           //一开始有0个产品
	pthread_create(&pid, NULL, ProducerMain, NULL);
	for (int i = 0; i < CONSUMER_COUNT; i++)
	{
		pthread_create(cid + i, NULL, ConsumerMain, NULL);
	}

	//join
	pthread_join(pid, NULL);
	for (int i = 0; i < CONSUMER_COUNT; i++)
	{
		pthread_join(cid[i], NULL);
	}
	return 0;
}
*/