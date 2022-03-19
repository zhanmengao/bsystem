#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>


void Catch(int signalID)
{
	printf("id=%u\n",signalID);
}

void printPending(sigset_t *set)
{
	for(int i=1;i<32;i++)
	{
		printf("%u",sigismember(set,i));
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	int i;
	signal(SIGALRM,Catch);
	sigset_t m_set;
	//信号集add
	if(sigaddset(&m_set,SIGQUIT)==0 && sigaddset(&m_set,SIGALRM)==0 && sigaddset(&m_set,SIGTSTP)==0)
	{
		sigset_t oldset;
		//更新m_set到阻塞信号集
		if(sigprocmask(SIG_BLOCK,&m_set,&oldset)==0)
		{
			printf("signal is mask\n");
		}
		else
		{
			printf("signal mask error\n");
			exit(-1);
		}
	}
	else
	{
		printf("signal mask error\n");
		exit(-1);
	}


	//信号集全部清零
	sigemptyset(&m_set);

	//设置闹钟
	struct itimerval newVal,oldVal;
	newVal.it_value.tv_sec=1.5f;
	newVal.it_value.tv_usec=0;
	newVal.it_interval.tv_sec=1;
	newVal.it_interval.tv_usec=0;
	setitimer(ITIMER_REAL,&newVal,&oldVal);
	sigset_t unjueset;
	while(1)
	{
		if(sigpending(&unjueset)==0)
		{
			//打印未决信号集
			printPending(&unjueset);
		}
		sleep(1);
	}
	return 0;
}