#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void* routineMain(void* data)
{
	printf("routine id = %lu\n",pthread_self());
}

int main(int argc, char const *argv[])
{
	pthread_t thread;
	printf("main routine id = %lu\n",pthread_self());

	int ret=pthread_create(&thread,NULL,routineMain,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create error! %s\n",strerror(ret));
		exit(-1);
	}	
	sleep(1);
	printf("main routine id = %lu\n",pthread_self());
	return 0;
}