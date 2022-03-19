#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
int count = 2000;    //剩余的票总数
void* RuntinueMain(void* para)
{
	while(count--)
	{
		pthread_mutex_lock(&mutex);
		int ran = rand() % 3;
		count -= ran;                 //卖票
		sleep(ran);
		count -= ran;                 //卖票			
		pthread_mutex_unlock(&mutex);
		printf("%s %u 张，剩余 %u 张\n","卖出",ran*2,count);
		sleep(rand() % 4);	
	}
}

int main(int argc, char const *argv[])
{
	pthread_t tid;
	if(pthread_create(&tid,NULL,RuntinueMain,NULL) == 0)
	{
		pthread_mutex_init(&mutex ,NULL);
		while(count--)
		{
			pthread_mutex_lock(&mutex);
			int ran = rand() % 3;
			count -= ran;                 //卖票
			sleep(ran);
			count -= ran;                 //卖票			
			pthread_mutex_unlock(&mutex);
		    printf("%s %u 张，剩余 %u 张\n","卖出",ran*2,count);			
			sleep(rand() % 4);	
		}
		pthread_cancel(tid);
		pthread_mutex_destroy(&mutex);
	}
	return 0;
}