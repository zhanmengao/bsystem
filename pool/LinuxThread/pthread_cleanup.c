#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

void cleanup(void* arg)
{
	printf("%ld cleanup call %d\n",pthread_self(),(int)(arg));
}
//主函数1，自行退出后被动执行
void* threadMain1(void* arg)
{
	int m = 1;
	printf("thrmain1 call \n");
	pthread_cleanup_push(cleanup,(void*)m);				//push stack
	printf("%ld thread end \n",pthread_self());
	pthread_exit(0);						//意外退出
	pthread_cleanup_pop(0);						//这句话必须有，但不会执行						
}

//主函数2,手动出栈后执行
void* threadMain2(void* arg)
{

	int m = 1,n = 2;
	printf("thrmain2 call \n");
        pthread_cleanup_push(cleanup,(void*)m);                         //push stack
        pthread_cleanup_push(cleanup,(void*)n);                         //push stack
	pthread_cleanup_pop(1);						//出栈清理函数，并执行
	printf("%ld thread end \n",pthread_self());
       	pthread_exit(0);                                                //意外退出
        pthread_cleanup_pop(0);
}

//主函数3，被cancel掉以后执行
void* threadMain3(void* arg)
{

        int i = 1;
        printf("thrmain3 call \n");
        pthread_cleanup_push(cleanup,(void*)i);                         //push stack
        printf("%ld thread end \n",pthread_self());
	while(1)
	{
		i++;
		printf("%ld wang to sleep %d\n",pthread_self(),i);
		sleep(1);
	}
	printf("this line will not run\n");
        pthread_cleanup_pop(0);
}

int main()
{
	pthread_t pid1,pid2,pid3;
	int ret = pthread_create(&pid1,NULL,threadMain1,NULL);
	if(ret)
	{
		printf("pthread_create failed : %s \n",strerror(ret));	
	}

	ret = pthread_create(&pid2,NULL,threadMain2,NULL);
        if(ret)
        {
                printf("pthread_create failed : %s \n",strerror(ret));
        }
	 
   	ret = pthread_create(&pid3,NULL,threadMain3,NULL);
        if(ret)
        {
                printf("pthread_create failed : %s \n",strerror(ret));
        }
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	sleep(2);
	pthread_cancel(pid3);
	pthread_join(pid3,NULL);
	printf("i am main thread,i alive\n");
	return 0;
}
