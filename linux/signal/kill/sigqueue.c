#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
/*
typedef struct siginfo_t{ 
int si_signo;           //信号编号 
int si_errno;          //如果为非零值则错误代码与之关联 
int si_code;           //说明进程如何接收信号以及从何处收到 
pid_t si_pid;          //适用于SIGCHLD，代表被终止进程的PID 
pid_t si_uid;          //适用于SIGCHLD,代表被终止进程所拥有进程的UID 
int si_status;        //适用于SIGCHLD，代表被终止进程的状态 
clock_t si_utime;    //适用于SIGCHLD，代表被终止进程所消耗的用户时间 
clock_t si_stime;    //适用于SIGCHLD，代表被终止进程所消耗系统的时间 
sigval_t si_value; 
int si_int; 
void * si_ptr; 
void*  si_addr; 
int si_band; 
int si_fd; 
};


typedef union sigval {

               int  sival_int;

               void *sival_ptr;

}sigval_t;
*/

//若传值，则可以接收
//若传指针，则不同进程不能接收（不同进程的0-4G互相独立）
void CatchInfo(int signal, siginfo_t *info, void *data)
{
    if(info!=NULL)
    {
        printf("int=%d\n",info->si_value.sival_int);
        if(info->si_value.sival_ptr)
        {
             //printf("val = %s\n",(char*)info->si_value.sival_ptr);
        }
    }
}

int main(int argc, char const *argv[])
{
    pid_t pid=getpid();

    //捕捉信号
    struct sigaction act;
    act.sa_sigaction=CatchInfo;
    act.sa_flags=SA_SIGINFO;
    sigfillset(&(act.sa_mask));
    sigaction(SIGUSR1,&act,NULL);

    //给自己发送信号
    union sigval para;
    para.sival_int = 9;
    sigqueue(pid,SIGUSR1,para);       //传值

    char buf[12];
    strcpy(buf,"aaaaaaa");
    para.sival_ptr = buf;                  //传指针
    sigqueue(pid,SIGUSR1,para);


    pid = fork();
    if(pid>0)
    {
        para.sival_int = 9;
        //给子进程发个信号
        sigqueue(pid,SIGUSR1,para);

        para.sival_ptr = buf;
        sigqueue(pid,SIGUSR1,para);
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
