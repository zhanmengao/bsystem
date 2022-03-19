#include<stdio.h>  
#include<stdlib.h>  
#include <pthread.h>  
#include <unistd.h> //sleep

void mycleanfunc(void *arg) //������
{
	printf("mycleanfunc:%d\n", *((int *)arg));						 
}
 
void *thfunc(void *arg)  
{  
	int i = 1;  
	printf("thread start-------- \n"); 
	pthread_cleanup_push(mycleanfunc, &i); //��������ѹջ
	while (1)  
	{
		i++;  
		printf("i=%d\n", i);
	}	
	printf("this line will not run\n");
	pthread_cleanup_pop(0);
	
	return (void *)0;  
}  
int main()  
{  
	void *ret = NULL;  
	int iret = 0;  
	pthread_t tid;  
	pthread_create(&tid, NULL, thfunc, NULL);  //�����߳�
	sleep(1);  
          
	pthread_cancel(tid); //����ȡ���̵߳�����  
	pthread_join(tid, &ret);  //�ȴ��߳̽���
	if (ret == PTHREAD_CANCELED) //�ж��Ƿ�ɹ�ȡ���߳�
		printf("thread has stopped,and exit code: %d\n", ret);  //��ӡ�·���ֵ��Ӧ����-1
	else
		printf("some error occured");
          
	return 0;  
}
