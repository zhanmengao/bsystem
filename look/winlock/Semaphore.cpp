#include "Define.h" 

unsigned WINAPI Read(void * arg);
unsigned WINAPI Accu(void * arg);

static HANDLE semOne;
static HANDLE semTwo;
static int num;

/*
�������á��ź���ֵΪ0ʱ����non-signaled״̬������0ʱ����signaled״̬�������Խ���ͬ����
��lInitialCount��������0ʱ������non-signaled״̬���ź�������
����lMaximumCount����3ʱ���ź������ֵΪ3����˿���ʵ��3���߳�ͬʱ�����ٽ���ʱ��ͬ����

�ź����������0ʱ��Ϊsignaled����Ϊ0ʱ��Ϊnon-signaled����
��ˣ�����WaitForSingleObject����ʱ���ź�������0������²Ż᷵�ء�
���ص�ͬʱ���ź�����ֵ��1��ͬʱ����non-signaled״̬��
*/
int mainSemaphore()
{
	HANDLE hThread1, hThread2;

	//�����ź�������,����Ϊ0����non-signaled״̬  
	semOne = CreateSemaphore(NULL, 0, 1, NULL);          //Ϊ0��ס���ʼ��1��

	//�����ź�����������Ϊ1����signaled״̬  
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);          //Ϊ1��ס���ʼ��1��

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);
	CloseHandle(semOne); //����  
	CloseHandle(semTwo); //����  

	return 0;
}

//������
unsigned WINAPI Read(void * arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		fputs("Input num: \n", stdout);

		//�ٽ����Ŀ�ʼ signaled״̬  
		WaitForSingleObject(semTwo, INFINITE);
		scanf("%d", &num);

		//�ٽ����Ľ��� non-signaled״̬
		ReleaseSemaphore(semOne, 1, NULL);
	}
	return 0;
}

//������
unsigned WINAPI Accu(void * arg)
{
	int sum = 0, i;
	for (i = 0; i < 5; i++)
	{
		//�ٽ����Ŀ�ʼ non-signaled״̬  
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		printf("sum = %d \n", sum);
		//�ٽ����Ľ��� signaled״̬  
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
//�����ߣ���������
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

//�����ߣ���������
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
	sem_init(&boxSem, 0, MAX);                          //һ��ʼ��max������
	sem_init(&itemSem, 0, 0);                           //һ��ʼ��0����Ʒ
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