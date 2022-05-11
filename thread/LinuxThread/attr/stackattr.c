#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define THREAD_STACK_SIZE 0x100000

int count = 0;

void *tfn(void *arg)
{
	while (1) 
	{
		sleep(10);
	}
    pthread_exit((void *)1);
}

int main(void)
{
	pthread_t tid;
	pthread_attr_t attr;

	while(1)
	{
	    //set
		char* stack = malloc(THREAD_STACK_SIZE);
		pthread_attr_setstack(&attr,stack,THREAD_STACK_SIZE);

	    //get
		int size = 0;
		char* retStack = NULL;
		pthread_attr_getstack(&attr,&retStack,&size);
		printf("%s %x %u\n","threaad stack addr =",retStack,size);

		pthead_attr_getstacksize(&attr,&size);
		printf("%s %u\n","Only Get Size = ",size);	
		if(pthread_create(&tid, &attr, tfn, NULL) == 0)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	printf("%s %u\n","count = ",count);
    pthread_attr_destroy(&attr);
	return 0;
}
