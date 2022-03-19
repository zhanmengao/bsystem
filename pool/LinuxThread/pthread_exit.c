#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void* routineMain(void* data)
{
	sleep((int)data);
	printf("I am %u,routine id = %lu\n",(int)data,pthread_self());
	if((int)data == 3)
	{
		exit(0);
	}
	else
	{
		return NULL;
	}
}

int main(int argc, char const *argv[])
{
	pthread_t thread;
	for(int i=0;i<5;i++)
	{
		int ret=pthread_create(&thread,NULL,routineMain,(void*)i);	
		if(ret != 0)
		{
			fprintf(stderr,"pthread_create error! %s\n",strerror(ret));
			exit(-1);
		}		
	}
	pthread_exit(NULL);      //exit main thread
}