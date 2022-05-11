#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static int globalData=100;
void* routineMain(void* data)
{
	globalData=1000;
}

int main(int argc, char const *argv[])
{
	pthread_t thread;
	printf("globalData = %u",globalData);
	int ret=pthread_create(&thread,NULL,routineMain,NULL);
	if(ret != 0)
	{
		fprintf(stderr,"pthread_create error! %s\n",strerror(ret));
		exit(-1);
	}
	sleep(1);
	printf("globalData = %u\n",globalData);	
	return 0;
}