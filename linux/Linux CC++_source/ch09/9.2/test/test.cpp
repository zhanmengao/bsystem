#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
 
int a = 200;
int b = 100;
 
void* ThreadA(void*)
{
	while (1)
	{
		a -= 50; //������ֵ50Ԫ�Ļ��� 
		b += 50;//�ջ�50ԪǮ
	}
}
 
void* ThreadB(void*)
{
	while (1)
	{
		printf("%d\n", a + b);
		sleep(1);    
	}
}
 
int main()
{
	pthread_t tida, tidb;
 
	pthread_create(&tida, NULL, ThreadA, NULL);
	pthread_create(&tidb, NULL, ThreadB, NULL);
	pthread_join(tida, NULL);
	pthread_join(tidb, NULL);
	return 1;
}