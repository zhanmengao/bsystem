#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void *tfn(void *arg)
{
	int n = 3;

	while (n--) 
	{
		printf("thread count %d\n", n);
		sleep(1);
	}

    pthread_exit((void *)1);
}

int main(void)
{
	pthread_t tid;
	pthread_attr_t attr;
	void *tret;
	int err;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, tfn, NULL);
	pthread_attr_getdetachstate(&attr,&err);
	if(err == PTHREAD_CREATE_DETACHED)
	{
		printf("%s\n","线程分离!");
	}
	else
	{
		printf("%s\n","线程未分离!");
	}
	while (1) 
	{
		err = pthread_join(tid, &tret);              //线程被分离，join无效
        printf("-------------err= %d\n", err);
		if (err != 0)
			fprintf(stderr, "thread_join error: %s\n", strerror(err));
		else
			fprintf(stderr, "thread exit code %d\n", (int)tret);

		sleep(1);
	}

	pthread_attr_destroy(&attr);
	return 0;
}
