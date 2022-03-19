#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lockA;
pthread_mutex_t lockB;
pthread_mutex_t lockC;
int count = 100;

void ShellTicks(int data)
{
	count -=data;
	printf("%s %u ，剩余%u\n","shell",data,count);
}
void* RoutinueMain(void* para)
{
	while(1)
	{
		if(pthread_mutex_trylock(&lockA) == 0)
		{
			if(pthread_mutex_trylock(&lockB) == 0)
			{
				int ran = rand() % 3;
				ShellTicks(ran);
				sleep(ran);
				pthread_mutex_unlock(&lockA);
				pthread_mutex_unlock(&lockB);					
			}
			else
			{
				pthread_mutex_unlock(&lockA);
			}
		}
		else
		{
			sleep(1);
			continue;
		}
		sleep(rand() % 5);			
	}
}


int main(int argc, char const *argv[])
{
	pthread_t tid;
	if(pthread_create(&tid,NULL,RoutinueMain,NULL) == 0)
	{
		while(1)
		{
			if(pthread_mutex_trylock(&lockA) == 0)
			{
                if(pthread_mutex_trylock(&lockB) == 0)                 //避免死锁：如果任意一个加锁失败，则放弃所有的锁
				{
					if(pthread_mutex_trylock(&lockC) != 0)
					{
					    pthread_mutex_unlock(&lockC);
					    pthread_mutex_lock(&lockC);	
					}
					int ran = rand() % 3;
					ShellTicks(ran);
                    sleep(1);
					pthread_mutex_unlock(&lockA);
					pthread_mutex_unlock(&lockB);
					pthread_mutex_unlock(&lockC);										
				}
				else
				{
					pthread_mutex_unlock(&lockA);
				}
			}
			else
			{
				sleep(1);
				continue;
			}
			sleep(rand() % 5);			
		}

	}
	return 0;
}
