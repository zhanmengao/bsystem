#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_rwlock_t rwlock;
int count = 200;    //剩余的票总数
void* RuntinueWrite(void* para)
{
	int i = (int)para;
    while(1)
	{
        pthread_rwlock_wrlock(&rwlock);
		sleep(rand() % 3);	
		if(count>0)
		{
			count--;                 //卖票	
			printf("%u %s\n",i ,"柜台卖出一张票！");
        	pthread_rwlock_unlock(&rwlock);
		}
		else
		{
			pthread_rwlock_unlock(&rwlock);
			pthread_exit(0);
		}
		sleep(rand() % 4);	
	}
}

void* RuntinueRead(void* para)
{
	int i = (int)para;	
    while(1)
	{
        pthread_rwlock_rdlock(&rwlock);
		printf("%u查询结果，还有%u张票\n",i,count);	
		if(count>0)
		{
        	pthread_rwlock_unlock(&rwlock);
		}
		else
		{
			pthread_rwlock_unlock(&rwlock);
			pthread_exit(0);
		}
		sleep(1);
	}
}

int main(int argc, char const *argv[])
{
	pthread_t tid[8];
	pthread_rwlock_init(&rwlock,NULL);
	for(int i = 0;i < 3;i++)
	{
	    pthread_create(&tid[i],NULL,RuntinueWrite,(void*)i);
	}
	for(int i = 0;i < 5;i++)
	{
	    pthread_create(&tid[i+3],NULL,RuntinueRead,(void*)i+3);
	}
	for(int i = 0;i<8;i++)
	{
        pthread_join(tid[i],NULL);
	}
	printf("%s\n","票卖完了!");
    pthread_rwlock_destroy(&rwlock);
	return 0;
}
