#include <pthread.h>
#include <stdio.h> 

void *thfunc(void *arg)
{
	int *pn = (int*)(arg); //��ȡ�����ĵ�ַ
	int n = *pn; 
	 
	printf("in thfunc:n=%d\n", n);
    return (void *)0; 
}
int main(int argc, char *argv [])
{
	pthread_t tidp;
	int ret, n=110;

	ret = pthread_create(&tidp, NULL, thfunc, &n);//�����̲߳�����n�ĵ�ַ
	if (ret)
	{
		printf("pthread_create failed:%d\n", ret);
		return -1;
	}
	 
	pthread_join(tidp,NULL); //�ȴ����߳̽���
	printf("in main:thread is created\n");
	 
	return 0;
}
