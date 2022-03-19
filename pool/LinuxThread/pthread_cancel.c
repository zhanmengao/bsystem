#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include<signal.h>
#include"errno.h"

typedef struct 
{
	int val;
	char buf[128];
}exit_t;

//通过return自行退出
void* routineMain_return(void* data)
{
	if(data!=NULL)
	{
		exit_t* retval =(exit_t*)data;
		retval->val=9;
	    strcpy(retval->buf,"Un Safe Ret!");
	    return 0;			
	}
	return (void*)-1;
}

//通过pthread_exit自行退出
void* routineMain_exit(void* data)
{
	if(data!=NULL)
	{
		exit_t *retval = (exit_t*)data;
		retval->val=9;
		strcpy(retval->buf,"Safe Ret!");	
		pthread_exit((void*)0);	
	}
	pthread_exit((void*)-1);
}

void* routineMain_cancel(void* data)
{
	while (1) 
	{
		sleep(1);
		pthread_testcancel();	//自己添加取消点*/
	}
}
void Catch(int sigid)
{
	printf("%u signal was catch,i will exit\n",sigid);
	pthread_exit(NULL);
}
void* routineMain_signal(void* data)
{
	signal(SIGQUIT,Catch);
	while (1) 
	{
		printf("%ld thread work ...\n",pthread_self());
		sleep(1);
	}
	return (void*)NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thread;
	int ret;
	exit_t *pRet=(exit_t *)malloc(sizeof(exit_t));

	//线程1
	pthread_create(&thread,NULL,routineMain_return,(void*)pRet);		
	pthread_join(thread,(void**)&ret);
	printf("one : val = %u,buf= %s ,ret= %u \n",pRet->val,pRet->buf,ret);


   	 //线程2
	pthread_create(&thread,NULL,routineMain_exit,(void*)pRet);		
	pthread_join(thread,(void**)&ret);
	printf("two : val = %u,buf= %s ,ret= %u \n",pRet->val,pRet->buf,ret);



	//线程3
	pthread_create(&thread, NULL, routineMain_cancel, NULL);
    pthread_cancel(thread);            		//发送取消该线程的请求，会在线程主控函数遇到检查点时，终止该线程
	pthread_join(thread,(void**)&ret);
	if(ret == PTHREAD_CANCELED)
	{
		printf("PTHREAD_CANCELED");
	}
	printf("three : val = %u,buf= %s ,ret= %u \n",pRet->val,pRet->buf,ret);

	//线程4
	pthread_create(&thread, NULL,routineMain_signal, NULL);
	sleep(2);				//休眠，等待子线程开始运行
	pthread_kill(thread,SIGQUIT);
	pthread_join(thread,(void**)&ret);
	printf("three : val = %u,buf= %s ,ret= %u \n",pRet->val,pRet->buf,ret);

	//检测线程存活
	int kill_rc = pthread_kill(thread,0);
	if(kill_rc == ESRCH)
	{
		printf("%s","the thread did not exists or already quit\n");
	}
	else
	{
        printf("%s","the thread is alive\n");
	}

	if(pRet != NULL)
	{
	    free(pRet);
	    pRet=NULL;		
	}	
}
