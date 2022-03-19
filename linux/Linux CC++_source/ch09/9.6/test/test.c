#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*��ʼ��������*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*��ʼ����������*/

void *thread1(void *);
void *thread2(void *);

int i = 1;
int main(void)
{
	pthread_t t_a;
	pthread_t t_b;

	pthread_create(&t_a, NULL, thread2, (void *)NULL);//�����߳�t_a
	pthread_create(&t_b, NULL, thread1, (void *)NULL); //�����߳�t_b
	pthread_join(t_b, NULL);/*�ȴ�����t_b����*/
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	exit(0);
}

void *thread1(void *junk)
{
	for (i = 1; i <= 20; i++)
	{
		pthread_mutex_lock(&mutex);//��ס������
		if (i % 3 == 0)
			pthread_cond_signal(&cond); //���ѵȴ���������cond���߳�
		else       
			printf("thead1:%d\n", i); //��ӡ��������3��i
		pthread_mutex_unlock(&mutex);//����������

		sleep(1);
	}

}

void *thread2(void *junk)
{
	while (i < 20)
	{
		pthread_mutex_lock(&mutex);

		if (i % 3 != 0)
			pthread_cond_wait(&cond, &mutex);//�ȴ���������
		printf("------------thread2:%d\n", i); //��ӡ������3��i
		pthread_mutex_unlock(&mutex);

		sleep(1);
		i++;
	}

}
