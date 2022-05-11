#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>

using namespace std;

#define MAXLEN 200000

#define NUM_THREADS 8

#define CAS __sync_bool_compare_and_swap
#define FAA __sync_fetch_and_add
#define FAS __sync_fetch_and_sub
#define VCAS __sync_val_compare_and_swap

struct node
{
	int num;
	node * next;
	node(int i, node* n) :
		num(i), next(n)
	{}
};


struct list
{
	node *head;
	node *tail;
	pthread_mutex_t  lock;
	int count;
};

void init(list *plist)
{
	node *tmp = new node(0, NULL);
	plist->head = tmp;
	plist->tail = tmp;
	plist->count = 0;
	pthread_mutex_init(&(plist->lock), NULL);
}



int enque_lock(list *plist, int num)
{
	node *tmp = new node(num, NULL);
	pthread_mutex_lock(&(plist->lock));

	plist->tail->next = tmp;
	plist->tail = tmp;
	plist->count++;
	pthread_mutex_unlock(&(plist->lock));
	return 0;
}
int enque(list *plist, int num)
{
	node *p = NULL;
	node *tmp = new node(num, NULL);
	do
	{
		p = plist->tail;
	} while (CAS(&(p->next), NULL, tmp) == false);
	CAS(&(plist->tail), p, tmp);

	FAA(&(plist->count), 1);
	return 0;
}


void  deque(list *plist)
{
	node *tmp = NULL;
	do
	{
		tmp = plist->head;
		if (plist->head == plist->tail)
		{
			printf("err\n");
			return;
		}
	} while (CAS(&(plist->head), tmp, tmp->next) == false);
	FAS(&(plist->count), 1);
	delete tmp;
	return;
}

void *SendMsg(void* p)
{
	struct timeval tv_begin, tv_end;
	gettimeofday(&tv_begin, NULL);
	int i = 0;
	while (i++ < MAXLEN)
	{
		enque((list*)p, i);
	}
	gettimeofday(&tv_end, NULL);
	long timeinterval = (tv_end.tv_sec - tv_begin.tv_sec) * 1000000 + (tv_end.tv_usec - tv_begin.tv_usec);
	printf("cost %llu us\n", timeinterval);
}


void *SendMsg2(void* p)
{
	struct timeval tv_begin, tv_end;
	gettimeofday(&tv_begin, NULL);
	int i = 0;
	while (i++ < MAXLEN)
	{
		enque_lock((list*)p, i);
	}
	gettimeofday(&tv_end, NULL);
	long timeinterval = (tv_end.tv_sec - tv_begin.tv_sec) * 1000000 + (tv_end.tv_usec - tv_begin.tv_usec);
	printf("cost %llu us\n", timeinterval);
}



int main(void)
{
	int rc, i;
	pthread_t thread[NUM_THREADS];
	list  ll;
	init(&ll);
	for (i = 0; i < NUM_THREADS; i++)
	{
		printf("Creating thread %i\n", i);
		rc = pthread_create(&thread[i], NULL, SendMsg, (void*)&ll);
		if (rc)
		{
			printf("ERROR; return code is %d\n", rc);
			return -1;
		}
	}

	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(thread[i], NULL);
	}

	while (ll.count > 0)
	{
		deque(&ll);
	}


	for (i = 0; i < NUM_THREADS; i++)
	{
		printf("Creating thread %i\n", i);
		rc = pthread_create(&thread[i], NULL, SendMsg2, (void*)&ll);
		if (rc)
		{
			printf("ERROR; return code is %d\n", rc);
			return -1;
		}
	}

	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(thread[i], NULL);
	}
	return 0;
}