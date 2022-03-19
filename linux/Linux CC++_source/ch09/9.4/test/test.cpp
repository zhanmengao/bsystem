#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
 
int a = 200; //��ǰ�����ֵ
int b = 100; //��ǰ�ֽ�
 
pthread_mutex_t lock; //����һ��ȫ�ֵĻ�����

void* ThreadA(void*) //��������߳�
{
	
	while (1)
	{
		pthread_mutex_lock(&lock);          //����
		a -= 50; //������ֵ50Ԫ�Ļ��� 
		b += 50;//�ջ�50ԪǮ
		pthread_mutex_unlock(&lock);    //����
	}

}
 
void* ThreadB(void*) //�ϰ�����߳�
{
	while (1)
	{
		pthread_mutex_lock(&lock);  //����
		printf("%d\n", a + b);
		pthread_mutex_unlock(&lock);  //����
		sleep(1);    
	}
}
 
int main()
{
	pthread_t tida, tidb;
	pthread_mutex_init(&lock, NULL); //��ʼ��������
	pthread_create(&tida, NULL, ThreadA, NULL); //������������߳�
	pthread_create(&tidb, NULL, ThreadB, NULL); //�����ϰ�����߳�
	pthread_join(tida, NULL);
	pthread_join(tidb, NULL);
	
	pthread_mutex_destroy(&lock); //���ٻ�����
	
	return 1;
}
