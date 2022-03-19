#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void Catch(int signalID)
{
	printf("id=%u\n",signalID);
}

int main(int argc, char const *argv[])
{
	int i;
	signal(SIGALRM,Catch);
	struct itimerval newVal,oldVal;
	newVal.it_value.tv_sec=3;
	newVal.it_value.tv_usec=0;
	newVal.it_interval.tv_sec=1;
	newVal.it_interval.tv_usec=0;
	setitimer(ITIMER_REAL,&newVal,&oldVal);
	while(1)
	{
		sleep(1);
	}
	return 0;
}