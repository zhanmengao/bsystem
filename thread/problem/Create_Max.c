#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>


void* Thread_Main(void* para)
{
	while(1)
	{
		sleep(10);
	}
	return (void*)0;
}


int main(int argc, char const *argv[])
{
	pthread_t pid;
	int count = 0;
	while(1)
	{
		if(pthread_create(&pid,NULL,Thread_Main,NULL) == 0)
		{
			count++;
		}
		else
		{
			printf("%s %u\n","max count =",count);
		}
	}
	return 0;
}