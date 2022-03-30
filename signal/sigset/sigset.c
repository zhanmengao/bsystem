#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void mysigismember(sigset_t *set,int signal)
{
	int	ret = sigismember(set,signal);
	if(ret == 0)
	{
		printf("%u %s\n",signal,"不在集合中");
	}
	else if(ret == 1)
	{
		printf("%u %s\n",signal,"在集合中");		
	}
	else if(ret == -1)
	{
		perror("sigismember 出错");
	}
}


int main(int argc, char const *argv[])
{
	sigset_t set;int ret;

	sigemptyset(&set);                                //清空信号集
	mysigismember(&set,SIGUSR1);

	sigaddset(&set,SIGUSR1);                        //往信号集中添加某个信号
	mysigismember(&set,SIGUSR1);

	sigdelset(&set,SIGUSR1);                       //往信号集中删除某个信号
	mysigismember(&set,SIGUSR1);

	sigfillset(&set);                            //将信号集的标志位全填1
	for(int i=1;i<31;i++)
	{
		mysigismember(&set,i);
	}

	return 0;
}