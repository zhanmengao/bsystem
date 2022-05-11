#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void* routineMain(void* data)
{
	int i=(int)data;
	printf("%u th , routine id = %lu\n",i,pthread_self());
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
	sleep(5);
	return 0;
}