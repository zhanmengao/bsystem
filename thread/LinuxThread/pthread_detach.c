#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct 
{
	int val;
	char buf[128];
}exit_t;


void *tfn(void *arg)
{
	int n = 3;

	while (n--) 
	{
		printf("thread count %d\n", n);
		sleep(1);
	}
	pthread_detach(pthread_self());         //让线程分离  ----自动退出,无系统残留资源
    pthread_exit((void *)1);
}

int main(void)
{
	pthread_t tid;
	void *tret;
	int err;

	pthread_create(&tid, NULL, tfn, NULL);
	//pthread_detach(tid);         //让线程分离  ----自动退出,无系统残留资源

	sleep(5);
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

	return 0;
}
