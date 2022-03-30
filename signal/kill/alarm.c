#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/time.h>

void Alarm(int sec)
{
	struct itimerval newVal,oldVal;
	newVal.it_value.tv_sec = sec;
	newVal.it_value.tv_usec=0;
	newVal.it_interval.tv_sec=0;
	newVal.it_interval.tv_usec=0;
	setitimer(ITIMER_REAL,&newVal,&oldVal);	
}
int main(int argc, char const *argv[])
{
	int i;
	alarm(1);
	for(i=1;;i++)
	{
		printf("%d\n",i);
	}
	return 0;
}