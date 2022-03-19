#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct 
{
	int val;
	char buf[128];
}exit_t;


void* routineMain_Safe(void* data)
{
	int i = (int)data;
	if(i == 1)
	{
		i=100;
	}
	else if(i == 3)
	{
		i=300;
	}
	pthread_exit((void*)i);
}


int main(int argc, char const *argv[])
{
	pthread_t thread_safe[5];
	int ret[5];

	//Create
	for(int i=0;i<5;i++)
	{
		pthread_create(&(thread_safe[i]),NULL,routineMain_Safe,(void*)i);			
	}

	//GC
	for(int i=0;i<5;i++)
	{	
		pthread_join((thread_safe[i]),(void**)(&(ret[i])));
		printf("thread %u i = %u\n",i,ret[i]);	
	}	

}