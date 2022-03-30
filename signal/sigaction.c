#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void Catch(int sigid)
{
    printf("%u signal was catch\n",sigid);
    sleep(5);
    printf("%s\n","catch finish");
}

int main()
{
    struct sigaction act,old;
    int ret;
    act.sa_handler=Catch;
    sigfillset(&(act.sa_mask));
    act.sa_flags=0;

    ret=sigaction(SIGINT,&act,&old);
    if(ret==-1)
    {
        perror("sigaction error");
        exit(-1);
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
