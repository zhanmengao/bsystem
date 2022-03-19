#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>


int main()
{
	int fd[3];
	pid_t pid;
	pid_t cpid[3];
	int ret=pipe(fd);
	if(ret==-1)
	{
		perror("pipe error\n");
		exit(22);
	}

	pid=fork();//1
	//child1 write:game server
	if(pid==0)
	{
		printf("create program 1 write,pid=%d\n",getpid());
		close(fd[0]);
	    dup2(fd[1],STDOUT_FILENO);//输出重定向:input to pipe

	    //read file
	    FILE* pFile=fopen("/home/uzi/桌面/gitSPace/系统编程/execlp.c","rb");
	    if(pFile)
	    {
	    	char buffer[1024];
	    	fseek(pFile, 0, SEEK_END);//ÒÆ¶¯µ½ÎÄ¼þÎ²
		    int nLen = ftell(pFile);
		    if (nLen > 0)
		    {
			   rewind(pFile);
			   fread(buffer, sizeof(unsigned char), nLen, pFile);
		    }
		    fclose(pFile);
			write(fd[1],buffer,nLen);
		    exit(1);
	    }
	    else
	    {
	    	char buffer[]={"don not find file\n"};
	    	write(1,buffer,strlen(buffer));
	    	close(fd[1]);
	    	exit(1);
	    }

	}
	else
	{
		cpid[0]=pid;
	}

	pid=fork();//2
	//child2 read:db 
	if(pid==0)
	{
		printf("create program 2 read,pid=%d\n",getpid());
		close(fd[1]);
		char buffer[1024];
		dup2(fd[0],0);//input重定向
		int ret;
		while((ret=read(0,buffer,1024))>0)
		{
			write(1,buffer,ret);
		}
		close(fd[0]);
		exit(2);
		
	}
	else
	{
		cpid[1]=pid;
	}


	pid=fork();
	//child3 write:db 
	if(pid==0)
	{
		printf("create program 3 write,pid=%d\n",getpid());
		close(fd[0]);
	    dup2(fd[1],STDOUT_FILENO);//输出重定向:input to pipe

	    execlp("ls","ls",NULL);
	    close(fd[1]);
	    exit(3);
	}
	else
	{
		cpid[2]=pid;
	}

	close(fd[0]);
	dup2(fd[1],STDOUT_FILENO);
	char buffer[1024];
	while((ret=read(0,buffer,1024))>0)
	 {
	   	write(1,buffer,ret);
	   	if(buffer[0]=='q')
	   	{
	   		break;
	   	}
	 }
	 close(fd[1]);

	int i;
	//parent gc
	for(i=0;i<3;i++)
	{
		pid=waitpid(-1,NULL,WNOHANG);
		if(pid)
		{
			printf("gc child %d\n", pid);
		}
	}
}
