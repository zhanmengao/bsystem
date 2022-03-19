#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>

void Catch(int signal)
{
	printf("catch signal %u\n",signal);
}

int SleepPause(float second)
{
	struct sigaction act,old;
	int ret;
	act.sa_handler=Catch;
	sigemptyset(&(act.sa_mask));
	act.sa_flags=0;

	ret=sigaction(SIGALRM,&act,&old);
	alarm(second);
	//signal(SIGALRM,Catch);
	if(pause()==-1)
	{
		printf("%s\n","pause return value = -1");

		ret=sigaction(SIGALRM,&old,NULL);
	}
	ret=alarm(0);
	return ret;
}

int SleepSIG(float second)
{
	struct sigaction act,old;
	int ret;

	//1 Catch signal
	act.sa_handler=Catch;
	sigemptyset(&(act.sa_mask));
	act.sa_flags=0;
	ret=sigaction(SIGALRM,&act,&old);

	//2 mask SIGALRM
    sigset_t newSet,oldSet;
    sigemptyset(&newSet);
    sigaddset(&newSet,SIGALRM);
    sigprocmask(SIG_BLOCK,&newSet,&oldSet);

    //3 alarm
	alarm(second);

    //4 make sigsuspend mask
    sigset_t suspendSet=oldSet;
    sigdelset(&suspendSet,SIGALRM);

    //5 pause to SIGALRM ----->>> 只有在sigsuspend被调用的时候,SIGALRM才被解除阻塞
    sigsuspend(&suspendSet);


    //还原
    sigaction(SIGALRM,&old,NULL);	
	ret=alarm(0);
	return ret;
}

int main(int argc, char const *argv[])
{
	while(1)
	{
	   SleepPause(2);
	   printf("%s\n","my sleep call over");		
	}

	return 0;
}