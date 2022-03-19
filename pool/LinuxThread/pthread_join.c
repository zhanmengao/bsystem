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


void* routineMain_Unsafe(void* data)
{
	exit_t retval;
	retval.val=9;
	strcpy(retval.buf,"Un Safe Ret!");
	//pthread_exit(retval);
}

void* routineMain_Safe(void* data)
{
	exit_t *retval = (exit_t*)data;
	retval->val=9;
	strcpy(retval->buf,"Safe Ret!");
	pthread_exit((void*)9);
}


int main(int argc, char const *argv[])
{
	pthread_t thread_safe;
	int ret;

	exit_t *safe_ret=(exit_t *)malloc(sizeof(exit_t));
	pthread_create(&thread_safe,NULL,routineMain_Safe,(void*)safe_ret);		
	pthread_join(thread_safe,(void**)&ret);

	printf("safe_ret: val = %u,buf= %s ,ret= %u \n",safe_ret->val,safe_ret->buf,ret);
	if(safe_ret != NULL)
	{
	    free(safe_ret);
	    safe_ret=NULL;		
	}	
}