#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//0：正在运行   -1:无子进程

//指定回收,轮询
void rmChild_1(pid_t pid)
{
  int status;
  pid_t res;
  while(1)
  {
    res=waitpid(pid,&status,WNOHANG);
    if(res)//若回收成功，返回子进程ID
    {
      printf("回收:%d\n",res);
      break;
    }
    sleep(1);
  }
}

//指定回收,阻塞
void rmChild_2(pid_t pid)
{
  int status;
  pid_t res;
  res=waitpid(pid,&status,0);//在此处阻塞
  printf("回收:%d\n",res);
}


//回收所有孩子:阻塞
void KillAll_1()
{
  int i;
  for(i=0;i<5;i++)
  {
    pid_t pid;
    pid=waitpid(-1,NULL,0);//相当于wait(NULL)，会在此处阻塞
    printf("回收%d\n",pid);
  }
}

//回收所有孩子:轮询
void KillAll_2()
{
  pid_t wpid;
  while((wpid=waitpid(-1,NULL,WNOHANG))!=-1)//返回-1说明已经没有子进程了
  {
    if(wpid!=0)  //返回0说明子进程还在运行
    {
      printf("回收%d\n",wpid);
    }
    sleep(1);
  }
}

int main()
{
	pid_t pid;
  pid_t cpid[5];
  int i;
  for(i=0;i<5;i++)
  {
    pid=fork();
    if(pid==-1)
    {
      perror("fork error\n");
      exit(1);
    }
    else if(pid==0)
    {
      sleep(i+1);
      printf("我是子进程,pid=%u,ppid=%u.\n",getpid(),getppid());
      break;
    }
    else
    {
     cpid[i]=pid;
   }
 }
 if(pid!=0)
 {
//rmChild_2(cpid[1]);
  KillAll_2();
}
return 0;
}
